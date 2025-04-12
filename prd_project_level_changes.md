# Norns Build System Modernization
## Product Requirements Document (PRD)

**Version:** 1.0  
**Date:** April 6, 2025  
**Author:** Engineering Team  
**Status:** Draft

## 1. Executive Summary

This document outlines requirements for modernizing the Norns build system by migrating from WAF to CMake. The current system has created tight coupling between components and inconsistencies across the project, limiting development efficiency and maintainability. This migration aims to improve developer experience, enhance maintainability, and allow for more modular development.

## 2. Problem Statement

The existing WAF-based build system:
- Creates tight coupling between components
- Lacks consistency across the project
- Has limited IDE integration
- Makes development across platforms challenging
- Complicates onboarding for new developers

## 3. Goals and Objectives

### 3.1 Primary Goals
- Replace WAF with CMake as the primary build system
- Establish a component-based architecture with clear dependencies
- Standardize testing framework across components
- Improve build configuration management
- Implement robust dependency management

### 3.2 Key Performance Indicators (KPIs)
- Reduced build time by 30%
- Decreased onboarding time for new developers by 50%
- Improved test coverage to >80%
- All components building successfully on Linux, macOS, and Raspberry Pi

## 4. Detailed Requirements

### 4.1 Migration to CMake

**Requirement:** Replace WAF with CMake as the primary build system.

**Justification:**
- Industry standard for C/C++ projects with excellent documentation
- Already partially used in the project (maiden-repl)
- Better IDE integration for development
- Extensive ecosystem of tools and plugins
- Cross-platform support for Linux, macOS, and Raspberry Pi

**Acceptance Criteria:**
- All components can be built using CMake
- Build instructions are clear and documented
- CI/CD pipeline uses CMake for builds

### 4.2 Component-Based Architecture

**Requirement:** Restructure the build system to support modular component building.

**Implementation Details:**
1. Create a top-level CMakeLists.txt with clear project structure
2. Implement separate build directories for each component:
   ```
   norns/
   ├── CMakeLists.txt  # Main project file
   ├── matron/
   │   └── CMakeLists.txt
   ├── crone/
   │   └── CMakeLists.txt
   ├── maiden-repl/
   │   └── CMakeLists.txt
   └── ...
   ```
3. Define clear dependency relationships between components
4. Enable selective component building with options like:
   ```
   cmake -DBUILD_MATRON=ON -DBUILD_CRONE=OFF ...
   ```

**Acceptance Criteria:**
- Each component can be built independently
- Dependencies between components are clearly defined
- Components can be selectively included/excluded in builds

### 4.3 Standardized Testing Framework

**Requirement:** Formalize the Unity test framework integration.

**Implementation Details:**
1. Create a unified testing structure for C/C++ components
2. Add CMake functions to simplify test creation:
   ```cmake
   add_norns_test(test_name source_files)
   ```
3. Configure CI to automatically run the tests
4. Generate test coverage reports

**Acceptance Criteria:**
- All components have unit tests
- Tests can be run using standard commands
- CI pipeline includes automated testing
- Code coverage reports are generated

### 4.4 Build Configuration Management

**Requirement:** Create standardized build configurations and platform-specific optimizations.

**Implementation Details:**
1. Create standardized build configurations:
   ```
   -DCMAKE_BUILD_TYPE=Release|Debug|RelWithDebInfo
   -DPLATFORM=CM3|PI3|DESKTOP
   ```
2. Implement platform-specific optimization flags:
   ```cmake
   if(PLATFORM STREQUAL "CM3")
     target_compile_options(target PRIVATE
       -mcpu=cortex-a53 -mtune=cortex-a53 -mfpu=neon-fp-armv8)
   endif()
   ```

**Acceptance Criteria:**
- Build configurations are consistent across components
- Platform-specific optimizations are applied automatically
- Documentation covers all build options

### 4.5 Dependency Management

**Requirement:** Implement robust dependency management.

**Implementation Details:**
1. Use CMake's `find_package()` for system dependencies
2. Add FetchContent for third-party libraries when appropriate
3. Create clear interface libraries for internal dependencies

**Acceptance Criteria:**
- All external dependencies are managed through CMake
- Dependencies are versioned and documented
- Build fails gracefully with clear error messages when dependencies are missing

## 5. Migration Strategy

### 5.1 Phase 1: Prototype (Completed)
- Create basic CMake structure
- Convert one component (maiden-repl) completely
- Document build patterns

### 5.2 Phase 2: Component Migration (Completed)
- Migrate matron, crone, and other components
- Implement unit test framework

### 5.3 Phase 3: WAF Removal & CMake Finalization
- Remove all WAF scripts and configurations
- Simplify build.sh to only support CMake
- Ensure all components build successfully with CMake-only builds

### 5.4 Phase 4: Documentation & User Migration Support
- Update all build documentation to reference only CMake
- Create migration guide for users of the old system
- Provide examples for common development workflows

### 5.5 Phase 5: Performance Optimization & Refinement
- Optimize build performance
- Refine dependency management
- Enhance testing infrastructure

## 6. Compatibility and Risk Management

### 6.1 Complete WAF Removal

Given the successful migration of all major components to CMake, the decision has been made to completely remove the WAF-based build system:
- Remove all WAF scripts and configurations from the codebase
- Simplify the build.sh script to only support CMake
- Update CI pipeline to only use CMake builds
- Provide migration guidance for users of the old system

### 6.2 WAF Removal Plan

#### 6.2.1 Files to Remove
- `waf` executable
- All `wscript` files throughout the project
- WAF-related configuration files

#### 6.2.2 Build Script Simplification
- Modify `build.sh` to remove WAF support while maintaining the same user interface
- Ensure all build operations (configure, build, clean, install, test) work with CMake only

#### 6.2.3 Risk Mitigation
- Comprehensive testing of all components with CMake-only builds
- Verification of all functionality before final WAF removal
- Documentation of any differences in behavior between the two build systems

### 6.3 Specific Component Requirements

#### 6.2.1 Softcut Integration
- Make softcut an independent library with its own build targets
- Allow standalone builds and tests

#### 6.2.2 SuperCollider Integration
- Standardize SC extension installation
- Allow optional SC dependency for desktop development

## 7. Documentation Requirements

- Update readme-setup.md with new build instructions
- Create dedicated build system documentation
- Provide examples for common development workflows
- Document best practices for adding new components

## 8. Success Metrics

The success of this migration will be measured by:
- Build time reduction
- Developer onboarding time
- Test coverage percentage
- Cross-platform build success rate
- Developer satisfaction survey

## 9. Implementation Guidelines

When implementing changes:
- Follow best practices for CMake
- Document all decisions made during the process
- Don't reference this PRD or any specific task in documentation or code comments
- Make single changes to the build system at a time
- Test and commit each change separately
- Maintain a changelog of build system modifications
- Don't consider a task complete until it has been tested and proven to work
- Upon completion of a task, update the task tracking table with the status and any relevant notes
- If a new task must be created, review the existing tasks, and update the table accordingly

## 10. Task Tracking

This section tracks the progress of implementation tasks for the build system modernization.

### 10.1 Task Status Categories
- **Not Started**: Task has been identified but work has not begun
- **In Progress**: Work is currently being done on this task
- **Review**: Task is complete and awaiting review/approval
- **Completed**: Task has been reviewed and integrated
- **Blocked**: Task cannot proceed due to dependencies or issues

### 10.2 Migration Tasks Progress

| Task ID  | Description                                           | Dependencies                 | Assigned To | Status      | Due Date | Notes                                                                                                                                                                                    |
| -------- | ----------------------------------------------------- | ---------------------------- | ----------- | ----------- | -------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| TASK-001 | Create top-level CMakeLists.txt                       | None                         |             | Completed   |          | Created initial CMakeLists.txt with component structure and build options                                                                                                                |
| TASK-002 | Ensure testing framework setup functional             | TASK-001                     |             | Completed   |          | Integrated Unity test framework and created example test                                                                                                                                 |
| TASK-003 | Create platform-specific configurations               | TASK-001                     |             | Completed   |          | Created toolchain files and platform configurations for DESKTOP, PI3, and CM3                                                                                                            |
| TASK-004 | Convert matron to CMake                               | TASK-001                     |             | Completed   |          | Created CMake build system for matron. Implemented tests for HAL and event system with proper mock implementations                                                                       |
| TASK-005 | Convert maiden-repl component to CMake if necessary   | TASK-001, TASK-003           |             | Completed   |          | Enhanced existing maiden-repl CMake configuration, standardized build setup, and added proper test infrastructure                                                                        |
| TASK-006 | Convert crone component to CMake                      | TASK-001, TASK-003           |             | Completed   |          | Enhanced existing crone CMake configuration, added Faust support detection, and implemented test infrastructure                                                                          |
| TASK-007 | Make sure softcut is an independent library           | TASK-001                     |             | Completed   |          | Made softcut an independent library with its own build targets. Updated crone to use the independent library.                                                                            |
| TASK-008 | Standardize SuperCollider extension integration       | TASK-006                     |             | Completed   |          | Created CMake-based SC extension integration with optional dependency handling and platform-specific installation                                                                        |
| TASK-009 | Create compatibility script                           | TASK-005, TASK-006           |             | Completed   |          | Created a compatibility script that provides a unified interface for building with either WAF or CMake during transition                                                                 |
| TASK-010 | Update CI pipeline for CMake builds                   | TASK-005, TASK-006, TASK-007 |             | Completed   |          | Created GitHub Actions workflow for CI with Linux, macOS, and Raspberry Pi builds. Added Docker-based build environments.                                                                |
| TASK-011 | Clean up and update build documentation               | All tasks                    |             | Not Started |          |                                                                                                                                                                                          |
| TASK-012 | Remove all WAF scripts and configurations             | TASK-004, TASK-005, TASK-006 |             | Completed   |          | Removed WAF support from the build system                                                                                                                                                |
| TASK-013 | Simplify build.sh to only support CMake               | TASK-009, TASK-012           |             | Completed   |          | Modified build.sh to remove WAF support while maintaining the same user interface                                                                                                        |
| TASK-014 | Create migration guide for users of the old system    | TASK-011, TASK-012           |             | Completed   |          | Created waf_to_cmake_migration.md with detailed instructions for transitioning from WAF to CMake                                                                                         |
| TASK-015 | Verify all functionality works with CMake-only builds | TASK-012, TASK-013           |             | Not Started |          | Comprehensive testing of all components with CMake-only builds                                                                                                                           |
| TASK-016 | Add tests for norns-centric softcut integration       | TASK-007                     |             | Not Started |          | Create tests for the integration between crone and softcut, focusing on norns usage                                                                                                      |
| TASK-017 | Complete removal of all WAF files and references      | TASK-020, TASK-021, TASK-022 |             | Not Started |          | Remove all remaining WAF files and ensure documentation is updated                                                                                                                       |
| TASK-018 | Move crone/lib/readerwriterqueue to third-party       | None                         |             | Completed   |          | Moved the readerwriterqueue library from crone/lib to third-party and updated all references to ensure proper CMake integration. Build verified working.                                 |
| TASK-019 | Project Finalization and Verification                 | All tasks                    |             | Not Started |          | Comprehensive verification that the entire Norns project has been successfully migrated to CMake with clean, readable, and consistent scripts throughout the project, free of errors     |
| TASK-020 | Convert watcher component to CMake                    | None                         |             | Completed   |          | Created CMakeLists.txt for the watcher component with proper dependency handling (libudev, libevdev, pthread). Added documentation for future HAL refactoring.                           |
| TASK-021 | Convert ws-wrapper component to CMake                 | None                         |             | Completed   |          | Created CMakeLists.txt for the ws-wrapper component with proper nanomsg dependency handling. Updated the top-level CMakeLists.txt to include the ws-wrapper component.                   |
| TASK-022 | Create CMakeLists.txt for third-party root directory  | None                         |             | Completed   |          | Created a CMakeLists.txt for the third-party root directory that properly includes all third-party libraries. Updated the top-level CMakeLists.txt to use the new third-party directory. |
| TASK-023 | Fix SuperCollider environment setup for tests         | TASK-008                     |             | Completed   |          | Updated test_sc_installation.cmake to create missing directories and handle missing files gracefully. Added helpful error messages and instructions for resolving issues.                |

### 10.3 Weekly Status Updates

**Week of April 6, 2025**
- Progress summary: Created the top-level CMakeLists.txt file that integrates the existing component-level build systems.
- Completed tasks: TASK-001
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-002 to set up the testing framework

**Week of April 7, 2025**
- Progress summary: Set up the Unity testing framework with an example test that successfully builds and runs. Created platform-specific configurations using toolchain files for DESKTOP, PI3, and CM3 platforms. Enhanced the maiden-repl CMake configuration by standardizing its build setup and adding proper test infrastructure. Enhanced the crone component's CMake configuration with proper Faust dependency handling and added test infrastructure.
- Completed tasks: TASK-002, TASK-003, TASK-005, TASK-006
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-007 to make softcut an independent library

**Week of April 14, 2025**
- Progress summary: Successfully converted matron to CMake including comprehensive testing capabilities. Created separate test runners for HAL and event system tests. Implemented proper mocking for HAL tests. Added detailed documentation for running tests.
- Completed tasks: TASK-004
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-007 to make softcut an independent library

**Week of April 21, 2025**
- Progress summary: Standardized SuperCollider extension integration with CMake. Created a dedicated CMakeLists.txt for SC extensions with platform-specific installation paths. Made SC dependency optional for desktop development. Added documentation and test files for SC extensions.
- Completed tasks: TASK-008
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-007 to make softcut an independent library and TASK-009 to create a compatibility script

**Week of April 28, 2025**
- Progress summary: Created a compatibility script that provides a unified interface for building Norns with either WAF or CMake during the transition period. The script supports all common build operations (configure, build, clean, install, test) and maps options between the two build systems. Created comprehensive documentation for using the script.
- Completed tasks: TASK-009
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-007 to make softcut an independent library and TASK-010 to update the CI pipeline for CMake builds

**Week of May 5, 2025**
- Progress summary: After reviewing the progress of the CMake migration, the decision was made to completely remove the WAF-based build system rather than maintaining it alongside CMake. This will simplify maintenance and provide a cleaner codebase. Updated the PRD to reflect this change in strategy and added new tasks related to WAF removal.
- Completed tasks: None
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-012 to remove all WAF scripts and configurations, and TASK-007 to make softcut an independent library

**Week of May 12, 2025**
- Progress summary: Completed the removal of WAF support from the build system. Simplified the build.sh script to only support CMake while maintaining the same user interface. Created a comprehensive migration guide (waf_to_cmake_migration.md) to help users transition from WAF to CMake. Updated build_compatibility.md to reflect the CMake-only approach.
- Completed tasks: TASK-012, TASK-013, TASK-014
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-007 to make softcut an independent library, TASK-010 to update the CI pipeline for CMake builds, and TASK-011 to update the build documentation

**Week of April 11, 2025**
- Progress summary: Created a GitHub Actions workflow for the CI pipeline that builds and tests the project on Linux, macOS, and Raspberry Pi. The workflow uses Docker for consistent build environments and leverages the build.sh script for building and testing. Added comprehensive testing for different build configurations and platforms. Added two new tasks to the project: moving the readerwriterqueue library to third-party and a comprehensive project finalization task to ensure all components are properly migrated to CMake with clean, consistent scripts. Conducted an analysis of remaining WAF components and identified three components that still need to be converted to CMake: watcher, ws-wrapper, and the third-party root directory. Added three new tasks (TASK-020, TASK-021, TASK-022) to address these conversions before proceeding with the complete removal of WAF files.
- Completed tasks: TASK-010
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-020, TASK-021, and TASK-022 to convert remaining components to CMake before completing TASK-017 (removal of all WAF files)

**Week of April 11, 2025 (continued)**
- Progress summary: Converted the watcher component to CMake. Created a CMakeLists.txt file that properly handles the component's dependencies (libudev, libevdev, pthread). Added comprehensive documentation for the watcher component, including details about its current implementation and suggestions for future improvements such as abstracting the Linux device handling into a Hardware Abstraction Layer (HAL) similar to what's done in other components. Updated the WAF to CMake Conversion Status section to reflect the completion of the watcher component conversion. Added a new task (TASK-023) to fix the SuperCollider environment setup for tests after observing test failures during verification.
- Completed tasks: TASK-020
- Tasks started: None
- Blockers encountered: SuperCollider extensions test failing due to missing directory
- Next steps: Proceed with TASK-021 and TASK-022 to convert the remaining components (ws-wrapper and third-party root directory) to CMake before completing TASK-017 (removal of all WAF files). Consider addressing TASK-023 to fix the SuperCollider environment setup for tests.

**Week of April 12, 2025**
- Progress summary: Converted the ws-wrapper component to CMake. Created a CMakeLists.txt file that properly handles the component's dependencies (nanomsg, pthread). Implemented manual detection of the nanomsg library to handle cases where pkg-config files are not properly set up. Updated the top-level CMakeLists.txt to include the ws-wrapper component and added it to the build.sh script as a selectable component. Updated the WAF to CMake Conversion Status section to reflect the completion of the ws-wrapper component conversion.
- Completed tasks: TASK-021
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-022 to convert the third-party root directory to CMake before completing TASK-017 (removal of all WAF files).

**Week of April 12, 2025 (continued)**
- Progress summary: Created a CMakeLists.txt for the third-party root directory that properly includes all third-party libraries (unity, softcut, link). Updated the top-level CMakeLists.txt to use the new third-party directory instead of including individual third-party libraries directly. Added proper conditional building of components based on project options. Added platform-specific configurations for the Link library. Updated the WAF to CMake Conversion Status section to reflect the completion of the third-party root directory conversion.
- Completed tasks: TASK-022
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-017 to complete the removal of all WAF files and references now that all components have been converted to CMake.

### 10.4 WAF to CMake Conversion Status

This section documents the current status of the WAF to CMake conversion process, which will be useful for completing TASK-017.

#### 10.4.1 Components with CMake Support

The following components have been successfully converted to CMake:

| Component     | CMakeLists.txt Location                       | Status    | Notes                                                          |
| ------------- | --------------------------------------------- | --------- | -------------------------------------------------------------- |
| matron        | /workspace/matron/CMakeLists.txt              | Completed | Fully converted with proper test infrastructure                |
| crone         | /workspace/crone/CMakeLists.txt               | Completed | Fully converted with Faust support and test infrastructure     |
| maiden-repl   | /workspace/maiden-repl/CMakeLists.txt         | Completed | Fully converted with test infrastructure                       |
| softcut       | /workspace/third-party/softcut/CMakeLists.txt | Completed | Converted as an independent library with its own build targets |
| unity (tests) | /workspace/third-party/unity/CMakeLists.txt   | Completed | Third-party testing framework with CMake support               |
| link          | /workspace/third-party/link/CMakeLists.txt    | Completed | Third-party library for synchronizing timing across devices    |
| sc            | /workspace/sc/CMakeLists.txt                  | Completed | SuperCollider extensions with platform-specific installation   |
| watcher       | /workspace/watcher/CMakeLists.txt             | Completed | Watchdog utility with documentation for future HAL refactoring |
| ws-wrapper    | /workspace/ws-wrapper/CMakeLists.txt          | Completed | Utility which wraps standard I/O in websockets                 |

#### 10.4.2 Components Requiring CMake Conversion

All components have been successfully converted to CMake. There are no remaining components that require conversion.

#### 10.4.3 WAF Files to Remove

Once all components have been converted to CMake, the following WAF-related files should be removed:

1. Root level:
   - `waf` executable
   - `wscript`

2. Component level wscript files:
   - `crone/wscript`
   - `maiden-repl/wscript`
   - `matron/wscript`
   - `third-party/wscript`
   - `watcher/wscript`
   - `ws-wrapper/wscript`

3. WAF lock files:
   - `.lock-waf_linux_build`
   - `third-party/.lock-waf_linux_build`
   - `third-party/build/.lock-waf_linux_build`
   - `matron/.lock-waf_linux_build`
   - `matron/build/.lock-waf_linux_build`

4. Documentation updates needed:
   - `readme-setup.md` contains references to WAF that need to be updated to reflect the CMake build system
