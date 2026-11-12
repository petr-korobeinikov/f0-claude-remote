# Spec Delta

## MODIFIED Requirements

### Requirement: Pairing is written down
`docs/guide/pairing.md` SHALL hold the pairing steps, moved from the README, and what follows them:
reconnecting, pairing again, and the radio, held only while the app is on Bluetooth.

#### Scenario: Moved
- **WHEN** `README.md` is read
- **THEN** it holds none of the page's steps

#### Scenario: Steps
- **WHEN** the page is read
- **THEN** it holds, in order, `mise run launch` not over the open Apps browser, a press if `Reconnect stopped` shows,
  Connect on `Claude Remote` in the Mac's Bluetooth settings, OK in `Verify code`
  and closing Keyboard Setup Assistant

#### Scenario: After pairing
- **WHEN** the page is read
- **THEN** it says the Mac connects by itself within a second of a drop,
  the app reconnects with no press when it opens again,
  past a minute with no link the device needs a press on `Reconnect stopped`,
  and pairing again starts by forgetting the device on the Mac

#### Scenario: The radio
- **WHEN** the page is read
- **THEN** it says the app holds the radio while on Bluetooth, so the phone app's connection waits,
  and hands it back on taking USB or leaving, the device's Bluetooth then standing as its setting says
