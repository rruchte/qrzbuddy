# Publishing QRZBuddy

The three platform build workflows run for changes to `main` and can be invoked manually. Pull requests are validated by `.github/workflows/release.yml`, which runs all three builds and verifies the packages without publishing a release.

## Create a release

After the release changes have been merged into `main` and verified, choose a new version tag:

```bash
git switch main
git pull --ff-only
git tag -a v1.1.0 -m "QRZBuddy v1.1.0"
git push origin v1.1.0
```

The older `v1.0.0` tag already exists and contains a Windows-only release. Use `v1.1.0` for the first multiplatform release, provided that tag is still unused. For subsequent releases, substitute a new version; never reuse an existing tag. Pushing a `v*` tag runs the coordinated release workflow. It builds Windows, macOS Intel, and Linux x64, waits for all three, checks that the packages exist, and creates a GitHub Release with generated notes.

Published files:
- `qrzbuddy-windows.zip` — portable Windows x64 distribution
- `qrzbuddy-macos-x64.dmg` — Intel macOS application (currently unsigned and unnotarized)
- `qrzbuddy-linux-x64.AppImage` — Linux x64 AppImage (built on Ubuntu 24.04)
- `SHA256SUMS` — SHA-256 digests of the packages

If any platform fails, the publication step does not run. Correct the issue and rerun the failed workflow or create a new tag for a subsequent release; avoid force-moving an already published version tag.

## Supported test coverage

CI verifies compilation and packaging, not complete interactive application behavior. Manual smoke tests should include QRZ authentication/callsign lookup, Qt WebEngine biography, Qt Location map, and JS8Call integration on each OS.

The macOS DMG is not code signed or notarized; Apple Gatekeeper may prevent the first launch. The Linux AppImage is built on Ubuntu 24.04 and is not guaranteed to run on older distributions with older glibc versions.

## Dependency cache

Build jobs cache Conan packages by operating system, architecture, and requirements hashes. When changing compiler/Qt versions or observing stale binary cache problems, bump `conan-v1` in the affected workflow cache key to force a clean dependency cache.
