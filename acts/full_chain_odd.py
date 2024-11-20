#!/usr/bin/env python3

import contextlib, acts, acts.examples
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

def generate_events(num_events: int, seed: int, outdir: str, 
                    g4_simulation: bool = False, ttbar: bool = True,
                    pileup: int = 200,
                    num_threads: int = 2):

    u = acts.UnitConstants

    geoDir = Path("/pscratch/sd/x/xju/LLMTracking/acts/thirdparty/OpenDataDetector")
    outname = f"ttbar_{num_events}evts_s{seed}_pu{pileup}"
    outdir = Path(outdir)
    outdir.mkdir(parents=True, exist_ok=True)
    outputDir = outdir / outname

    # acts.examples.dump_args_calls(locals())  # show python binding calls

    oddMaterialMap = geoDir / "data/odd-material-maps.root"
    oddDigiConfig = geoDir / "config/odd-digi-geometric-config.json"
    oddSpacepointSel = geoDir / "config/odd-sp-config.json"

    oddMaterialDeco = acts.IMaterialDecorator.fromFile(oddMaterialMap)

    detector, trackingGeometry, decorators = getOpenDataDetector(
        odd_dir=geoDir, mdecorator=oddMaterialDeco
    )
    field = acts.ConstantBField(acts.Vector3(0.0, 0.0, 2.0 * u.T))
    rnd = acts.examples.RandomNumbers(seed=seed)

    # TODO Geant4 currently crashes with FPE monitoring
    #with acts.FpeMonitor() if not g4_simulation else contextlib.nullcontext():
    s = acts.examples.Sequencer(
        events=num_events,
        numThreads=num_threads,
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

    if not ttbar:
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
            npileup=pileup,
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
            preSelectParticles=(
                ParticleSelectorConfig(
                    rho=(0.0, 24 * u.mm),
                    absZ=(0.0, 1.0 * u.m),
                    eta=(-3.0, 3.0),
                    pt=(150 * u.MeV, None),
                    removeNeutral=True,
                )
                if ttbar
                else ParticleSelectorConfig()
            ),
            enableInteractions=True,
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


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="Generate events")
    parser.add_argument(
        "-n",
        "--num-events",
        type=int,
        default=100,
        help="Number of events to generate",
    )
    parser.add_argument(
        "-s",
        "--seed",
        type=int,
        default=42,
        help="Seed for the random number generator",
    )
    parser.add_argument(
        "-o",
        "--outdir",
        type=str,
        default=".",
        help="Output directory for the generated events",
    )
    parser.add_argument("-m", "--pileup", type=int, default=200, help="Number of pileup events")
    parser.add_argument(
        '-t',
        "--num-threads",
        type=int,
        default=2,
        help="Number of threads to use in the simulation"
    )
    args = parser.parse_args()
    generate_events(args.num_events, args.seed, args.outdir,
                    pileup=args.pileup,
                    num_threads=args.num_threads)
