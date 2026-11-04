# Spec Delta

## ADDED Requirements

### Requirement: Exit sends no key
Exit, and a press with no action, SHALL send no key.

#### Scenario: Exit
- **WHEN** exit acts
- **THEN** no key is sent

#### Scenario: A press with no action
- **WHEN** Left or Back is tapped
- **THEN** no key is sent
