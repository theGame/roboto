# Copilot instructions for roboto

## Build, test, and lint

This repository does not include a package manager, Makefile, CI workflow, or automated test suite. The project is an Arduino sketch-based codebase centered on `obstacle-avoidance.ino`.

Use the Arduino IDE or VS Code Arduino extension to build and upload the sketch to the target board. The README notes the project targets Arduino Uno / ESP32-class hardware.

If you need to compile from the command line with Arduino CLI, use a board-specific fqbn such as:

```bash
arduino-cli compile --fqbn arduino:avr:uno .
```

or:

```bash
arduino-cli compile --fqbn esp32:esp32:esp32 .
```

There are no repository-defined lint or unit-test commands. When validating a change, compile the sketch and verify behavior via the serial output and hardware test run on the robot.

## High-level architecture

The project is organized as a small robotics firmware application split across hardware driver modules and behavior logic:

- `obstacle-avoidance.ino` is the entry point. `setup()` initializes the servo, ultrasonic sensor, motors, and gyroscope. `loop()` selects the active behavior strategy.
- `sample.h` / `sample.cpp` holds the higher-level robot maneuvers and is the place to look for the operational behavior (`avoidObstacles`, `rotateUsingGyroscope`, `chickenHeadManeuver`).
- `navigation.h` / `navigation.cpp` owns path selection and motion commands. `Navigation::decideDirection()` chooses the next direction based on the scanned distances, and `Navigation::drive()` translates that decision into motor directions and rotation behavior.
- `distances.h` / `distances.cpp` defines the `Distances` struct and the five-direction scan (`right`, `right45`, `front`, `left45`, `left`). This is the central object for obstacle detection and safety checks.
- `direction.h` / `direction.cpp` tracks the robot's current rotation state and target yaw. This logic is intentionally stateful; it prevents the robot from drifting mid-turn or accidentally losing the original turn target.
- Hardware wrappers are separated into single-purpose modules:
  - `my-motor.*` handles the motor driver, PWM, and direction bitmask commands.
  - `my-servo.*` controls the servo that swings the ultrasonic sensor.
  - `my-ultrasonic-sensor.*` reads raw distances and applies a `SimpleKalmanFilter` to smooth driving-distance measurements.
  - `my-giroscope.*` wraps `MPU6050_light` and exposes yaw (Z angle) updates.

The overall pattern is: sensor inputs -> distance/direction state -> navigation decision -> motor/servo commands. Keep behavior changes in the `sample`/`navigation` layer instead of burying logic in the hardware drivers.

## Key conventions

- This codebase is Arduino C++ rather than standard desktop C++; use `delay()`, `Serial`, `pinMode()`, `analogWrite()`, and the Arduino headers as the default style.
- The project uses global singleton instances for shared state, especially `distances`, `direction`, `navigation`, and `giroscope`. Keep that pattern unless there is a clear reason to refactor it.
- Keep low-level hardware pin assignments and board-specific setup in the dedicated `my-*` files. The top-level sketch should stay thin and delegate to behavior modules.
- The robot's motion logic is rotation-aware. If a direction turn is already in progress, the code deliberately keeps using the original rotation target rather than recalculating mid-turn. Preserve that behavior when editing navigation logic.
- Sensor scanning intentionally includes delays to let the servo settle and the ultrasonic readings stabilize. Timing-sensitive changes should account for those delays rather than removing them casually.
- Debug output is done through `Serial.println()`. When diagnosing behavior, use the existing serial logs to reason about distances, chosen direction, and rotation state.
- `#pragma once` is used in the headers; follow that style for new header files.

## Additional repo notes

- No `AGENTS.md`, `CLAUDE.md`, `.cursorrules`, `.windsurfrules`, or similar AI instruction files were present in this repository.
- The repo is intentionally small and hardware-focused; prioritize understanding the interaction between the sensors and motion state before making changes.
