#!/usr/bin/env python3
import os, argparse, pathlib, contextlib, acts, acts.examples
from pathlib import Path

from acts.examples.simulation import (
    addParticleGun,
    MomentumConfig,
    EtaConfig,
    ParticleConfig,
    addPythia8,
    addFatras,
    addGeant4,
    ParticleSelectorConfig,
    addDigitization,
)
from acts.examples import CsvTrackingGeometryWriter

from acts.examples.reconstruction import addSpacePointsMaking

from acts.examples.odd import getOpenDataDetector

parser = argparse.ArgumentParser(description="Full chain with the OpenDataDetector")

parser.add_argument("--events", "-n", help="Number of events", type=int, default=100)
parser.add_argument(
    "--geant4", help="Use Geant4 instead of fatras", action="store_true"
)
parser.add_argument("--seed", help="set RNG seed", default=42, type=int)
parser.add_argument("--outdir", help="output directory", default="odd_output")
parser.add_argument("-t", "--threads", help="# of threads", default=1, type=int)

args = vars(parser.parse_args())

ttbar_pu200 = True
g4_simulation = args["geant4"]

u = acts.UnitConstants

geoDir = Path("../thirdparty/OpenDataDetector")
outdir = "{}_ttbar_{}evts_s{}".format(args["outdir"], args["events"], args["seed"])
outputDir = pathlib.Path.cwd() / args["outdir"]
print("output directory: ", outputDir)

# acts.examples.dump_args_calls(locals())  # show python binding calls

oddMaterialMap = geoDir / "data/odd-material-maps.root"
oddFieldMap = geoDir / "data/odd-field.root"
oddDigiConfig = geoDir / "config/odd-digi-smearing-config.json"
oddSeedingSel = geoDir / "config/odd-seeding-config.json"

oddSpacepointSel = geoDir / "config/odd-sp-config.json"

oddMaterialDeco = acts.IMaterialDecorator.fromFile(oddMaterialMap)

detector, trackingGeometry, decorators = getOpenDataDetector(
    geoDir, mdecorator=oddMaterialDeco
)
field = acts.ConstantBField(acts.Vector3(0.0, 0.0, 2.0 * u.T))
rnd = acts.examples.RandomNumbers(seed=args["seed"])

# TODO Geant4 currently crashes with FPE monitoring
#with acts.FpeMonitor() if not g4_simulation else contextlib.nullcontext():
with contextlib.nullcontext():
    s = acts.examples.Sequencer(
        events=args["events"],
        numThreads=args["threads"],
        outputDir=str(outputDir),
        trackFpes=False,
    )

    # geo writer
    geo_writer =  CsvTrackingGeometryWriter(
        level=acts.logging.INFO,
        trackingGeometry=trackingGeometry,
        outputDir=str(outputDir),
        writePerEvent=False)
    s.addWriter(geo_writer)

    if not ttbar_pu200:
        addParticleGun(
            s,
            MomentumConfig(1.0 * u.GeV, 10.0 * u.GeV, transverse=True),
            EtaConfig(-3.0, 3.0, uniform=True),
            ParticleConfig(2, acts.PdgParticle.eMuon, randomizeCharge=True),
            vtxGen=acts.examples.GaussianVertexGenerator(
                stddev=acts.Vector4(
                    0.0125 * u.mm, 0.0125 * u.mm, 55.5 * u.mm, 5.0 * u.ns
                ),
                mean=acts.Vector4(0, 0, 0, 0),
            ),
            multiplicity=50,
            rnd=rnd,
        )
    else:
        addPythia8(
            s,
            hardProcess=["Top:qqbar2ttbar=on"],
            npileup=200,
            vtxGen=acts.examples.GaussianVertexGenerator(
                stddev=acts.Vector4(
                    0.0125 * u.mm, 0.0125 * u.mm, 55.5 * u.mm, 5.0 * u.ns
                ),
                mean=acts.Vector4(0, 0, 0, 0),
            ),
            rnd=rnd,
            # outputDirRoot=outputDir,
            outputDirCsv=outputDir,
        )
    if g4_simulation:
        if s.config.numThreads != 1:
            raise ValueError("Geant 4 simulation does not support multi-threading")

        # Pythia can sometime simulate particles outside the world volume, a cut on the Z of the track help mitigate this effect
        # Older version of G4 might not work, this as has been tested on version `geant4-11-00-patch-03`
        # For more detail see issue #1578
        addGeant4(
            s,
            detector,
            trackingGeometry,
            field,
            preSelectParticles=ParticleSelectorConfig(
                eta=(-3.0, 3.0),
                absZ=(0, 1e4),
                rho=(0, 1e3),
                pt=(150 * u.MeV, None),
                removeNeutral=True,
            ),
            # outputDirRoot=outputDir,
            outputDirCsv=outputDir,
            rnd=rnd,
        )
    else:
        addFatras(
            s,
            trackingGeometry,
            field,
            preSelectParticles=ParticleSelectorConfig(
                eta=(-3.0, 3.0),
                pt=(150 * u.MeV, None),
                removeNeutral=True,
            )
            if ttbar_pu200
            else ParticleSelectorConfig(),
            # outputDirRoot=outputDir,
            outputDirCsv=outputDir,
            rnd=rnd,
        )

    addDigitization(
        s,
        trackingGeometry,
        field,
        digiConfigFile=oddDigiConfig,
        # outputDirRoot=outputDir,
        outputDirCsv=outputDir,
        rnd=rnd,
    )

    # make spacepoints from the measurements
    addSpacePointsMaking(s, trackingGeometry, oddSpacepointSel)

    # add spacepoint writer
    s.addWriter(
        acts.examples.CsvSpacepointWriter(
            inputSpacepoints="spacepoints",
            outputDir=str(outputDir),
            level=acts.logging.INFO
        )
    )

    s.run()
