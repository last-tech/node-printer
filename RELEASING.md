# Releasing

This package ships **prebuilt native binaries** (via [`prebuild`](https://github.com/prebuild/prebuild) / [`prebuild-install`](https://github.com/prebuild/prebuild-install)) attached to a GitHub Release, and a source tarball on npm. Releases are **driven by version tags** — pushing to `main` does **not** publish anything.

## TL;DR — cut a release

```sh
# on an up-to-date main, working tree clean
npm version patch      # or: minor | major  → bumps package.json, commits, creates tag vX.Y.Z
git push --follow-tags # pushes the commit AND the tag
```

Pushing the `vX.Y.Z` tag is what triggers publishing. That's the whole ritual.

## What happens on a tag push

1. **`prebuild-main.yml`** (`on: push: tags: ['v*']`) runs on Windows `x64` and `ia32`. It builds the native addon for every supported Node and Electron target and `prebuild -u` uploads the binaries to the GitHub Release `vX.Y.Z` (creating the release if needed).
2. **`publish.yml`** (`on: release: published`) runs `npm publish --provenance`. The npm tarball contains **no** binaries — `prebuild-install` downloads the matching binary from the GitHub Release at install time.

> The tag name must match `package.json`'s version (`npm version` guarantees this), because `prebuild` uploads to the release named after the version.

## The three workflows

| Workflow | Trigger | Does |
|----------|---------|------|
| `prebuild-pr.yml` | `pull_request` | Builds all targets (Windows x64/ia32) to prove they compile. **No upload, no publish.** |
| `prebuild-main.yml` | `push` of a `v*` tag, or manual `workflow_dispatch` | Builds all targets and **uploads** prebuilt binaries to the `vX.Y.Z` GitHub Release. |
| `publish.yml` | GitHub `release: published`, or manual `workflow_dispatch` | `npm publish` the package. |

**Why tags, not `main` pushes:** previously `prebuild-main.yml` ran on every push to `main`, so any commit re-triggered a publish against whatever version was in `package.json`. Tag-driven publishing makes releasing explicit and keeps ordinary commits side-effect free.

## Supported targets

Targets are enumerated in the `Prebuild Node` / `Prebuild Electron` steps of `prebuild-main.yml` (and mirrored in `prebuild-pr.yml`).

- **Node.js:** 22, 24, 26
- **Electron:** 31–43

The ABI is constant across an Electron major, so each target is pinned to a stable `X.0.0` release (Electron 42 uses `42.3.0`, the earliest 42 whose headers compile on MSVC). When a new major reaches a **stable** release, add it as a new `-t X.0.0` line. Avoid prerelease (`-alpha`/`-beta`) targets: their headers can be broken and their ABI can shift before the stable release.

## Manual / recovery

- **Re-run prebuilds for an existing tag:** Actions → *Prebuild Binaries and Publish* → *Run workflow* (`workflow_dispatch`).
- **Re-publish to npm:** Actions → *Publish Package to npmjs* → *Run workflow*.
- **Partial release** (some assets missing after a failed run): re-run `prebuild-main.yml`; `prebuild` skips assets that already exist and uploads the rest.
