# Technical Debt

This document captures the main issues and improvements that would make the robot project more reliable, easier to maintain, and easier to adapt to different hardware setups.

## Summary

The project works as a small Arduino robotics prototype, but several parts are still tightly coupled to specific hardware behavior, timing assumptions, and control flow patterns. The biggest risks are around blocking delays, hard-coded configuration, limited sensor validation, and state management in the driving logic.

## Prioritized technical debt

### P1 — Replace blocking delays with a state-driven control loop

- Problem: Many actions rely on `delay()` in the main control flow, including motor commands, servo movement, and scan intervals.
- Why it matters: `delay()` blocks the entire sketch and prevents responsive behavior. While the robot is sleeping, it cannot react to sensor changes or recover from unexpected conditions.
- Impact: Increased likelihood of missed obstacles, poor responsiveness, and inconsistent movement.
- Suggested fix: Convert motion and sensor operations to a `millis()`-based state machine or a simple scheduler that tracks when each action should happen.
- Effort: Medium

### P1 — Add validation for sensor readings and calibration thresholds

- Problem: Ultrasonic and gyroscope values are used directly without validation or sanity checks.
- Why it matters: Sensor noise, invalid pulses, and startup/transient conditions can produce unrealistic readings such as 0 cm, very large values, or impossible angles.
- Impact: Wrong obstacle decisions, false positives, or endless rotation loops.
- Suggested fix: Add minimum/maximum bounds, reject invalid pulse readings, and define calibration or default values for sensor offsets.
- Effort: Small to Medium

### P1 — Centralize hardware constants and tuning values

- Problem: Pins, safe distances, delays, servo settings, and movement speeds are hard-coded across multiple files.
- Why it matters: Changes for a different board or a different robot build require editing multiple spots and increase the risk of inconsistent configuration.
- Impact: Reduced portability and higher chance of wiring or tuning mismatches.
- Suggested fix: Create a single configuration header or constants file for pin numbers, PWM values, safe distance thresholds, and timing settings.
- Effort: Small

### P2 — Simplify and document rotation state management

- Problem: Rotation behavior depends on mutable state in `Direction` and `Navigation`, and the logic is spread between multiple classes and methods.
- Why it matters: The project already contains comments that mention rotation drift and missing target checks. This is a sign the state transitions are easy to misread and hard to validate.
- Impact: Bugs in turning logic may be difficult to reproduce and debug.
- Suggested fix: Document the expected yaw-target state machine and make rotation transitions explicit and easier to reason about.
- Effort: Medium

### P2 — Remove dead or unfinished code paths

- Problem: Some functions or branches appear unfinished or not actively used, such as `Navigation::continueRotation()` and commented-out behavior in the loop.
- Why it matters: Unused code increases confusion and makes it harder to know which path is the canonical implementation.
- Impact: Extra maintenance burden and uncertainty during refactoring.
- Suggested fix: Either remove the dead code or complete it with a clear purpose and tests/hardware verification.
- Effort: Small

### P2 — Improve separation of concerns between hardware drivers and behavior logic

- Problem: Behavior code (`sample.cpp`) still directly orchestrates steering and motion decisions while hardware wrappers manage pins and low-level protocols.
- Why it matters: This is manageable for a small project, but as logic grows it becomes harder to reason about sensor inputs, command generation, and low-level effects.
- Impact: Increased complexity during future feature additions.
- Suggested fix: Keep `sample.cpp` focused on high-level maneuvers and keep configuration and helper decisions in `navigation` or dedicated controller modules.
- Effort: Small to Medium

### P3 — Add structured debug output and runtime diagnostics

- Problem: Logging is broad and repeated with `Serial.println()` statements, but there is no structured debug mode or central diagnostics pattern.
- Why it matters: Debugging real hardware behavior is difficult without consistent, filterable output.
- Impact: Longer diagnosis time for sensor and navigation issues.
- Suggested fix: Add debug flags, log only meaningful state transitions, and provide periodic summaries for sensor values and chosen direction.
- Effort: Small

### P3 — Add a board compatibility and setup checklist

- Problem: The repository notes Arduino Uno / ESP32 support, but the pin assignments and setup assumptions are spread throughout the code.
- Why it matters: A hardware-specific project becomes harder to port when the board assumptions are not explicit.
- Impact: Longer onboarding and more support overhead for new contributors or hardware rebuilds.
- Suggested fix: Document required board(s), sensor wiring, pin mapping, and any calibration steps needed to get the system running.
- Effort: Small

## Recommended roadmap

1. Stabilize sensor and motion behavior by validating readings and reducing blocking delays.
2. Centralize tuning and hardware configuration.
3. Clean up rotation state and remove dead code.
4. Improve debug diagnostics and setup documentation.
5. Add a lightweight verification flow for hardware behaviors as the project grows.

## Notes

This file is intentionally focused on repository health and maintainability. It highlights the most relevant engineering debt, but it should be updated as the project evolves and as new risks appear.
