# Proposal

## Why

`mise exec -- openspec validate --specs --strict` passes 25 specs of 27 and fails two,
each on one warning, `Purpose section is too brief (less than 50 characters)`:
`app-screen`'s Purpose, `Shows the remote's one screen.`, holds 30 characters (`openspec/specs/app-screen/spec.md:5`),
and `release`'s, `Cuts a release and publishes its GitHub Release.`, 48 (`openspec/specs/release/spec.md:5`).
Both were written before `openspec/config.yaml:181` asked 50 characters of a new spec's Purpose (`1cd45b4`),
and every plan's last check compares this command's flags before and after its archives,
so the two stand in each comparison as noise.
No plan is queued, and nothing else waits on it.

Decisions:

- **Each Purpose is rewritten in the task, straight in `openspec/specs/`,**
  not carried by a delta:
  OpenSpec 1.14.0's archive ignores a delta's Purpose for a spec that exists,
  warning `delta Purpose ignored; … Edit … directly to change it`
  (`dist/core/specs-apply.js:238-249` in the pinned package).
- **No spec delta: `.openspec.yaml` sets `skip_specs: true`,**
  as no requirement changes;
  the Purpose states what the requirements below it already hold.
- **Each Purpose names what its spec's requirements cover, in the form of the other 25,**
  one sentence, the capability's verb first:

  | Spec | Purpose |
  | --- | --- |
  | `app-screen` | Shows the remote's one screen: the link's status and every button's action, with boards of their own for voice, a voice cut by the link, a stopped offer and USB. |
  | `release` | Cuts a release by one command on `develop` and publishes its GitHub Release from the pushed tag, a draft until every job has passed. |

- **No check is added to CI or the hooks:**
  neither runs `openspec validate` today,
  and `config.yaml:181` already holds a new spec to 50 characters.
- **Relatively simple, so implemented on `develop`:**
  two lines of two specs, no code, no requirement.
- **It builds on `develop` alone:** no plan is queued.
- **It lands as `docs(openspec): fill out two spec purposes`.**

## What Changes

- `openspec/specs/app-screen/spec.md:5` and `openspec/specs/release/spec.md:5` take the Purposes above.
- `openspec validate --specs --strict` passes all 27 specs.

## Capabilities

### New Capabilities

None.

### Modified Capabilities

None: the change sets `skip_specs: true`.

## Impact

`openspec/specs/app-screen/spec.md` and `openspec/specs/release/spec.md`, one line each.
The code, the tests, `README.md` and `docs/` stay as they are:
`git grep` finds either old Purpose only in the two specs and in the archived changes that created them,
which stay as written.
