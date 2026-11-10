# Spec Delta

## Purpose

Tells whoever weighs the app's risk, in `SECURITY.md`,
how the device reaches the Mac and what a press does there.

## ADDED Requirements

### Requirement: SECURITY.md names every way the device reaches the Mac
`SECURITY.md`'s `## What the App Can Do` SHALL name each link the application takes
and how the Mac comes to trust it,
and for USB the name and IDs `usb-link` gives, with the IDs' owner.

#### Scenario: Both links
- **WHEN** `SECURITY.md`'s `## What the App Can Do` is read
- **THEN** it names Bluetooth, paired once in the Mac's settings and confirmed on the device,
  and USB, paired with nothing: a keyboard to the computer at the cable's other end,
  `Claude Remote` on Logitech's IDs `046D:C529`, that a Mac laptop with Apple silicon asks to allow first

### Requirement: SECURITY.md says a press has no lock
`SECURITY.md`'s `## What the App Can Do` SHALL say that a press acts on whatever the Mac has focused,
and that with no lock in the app, whoever holds the device holds its keys.

#### Scenario: No lock
- **WHEN** `SECURITY.md`'s `## What the App Can Do` is read
- **THEN** it says a press acts on whatever the Mac has focused,
  that a stray tap on OK can send a draft or approve a permission prompt,
  and that whoever holds the device holds those keys
