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

### 5.1 Phase 1: Prototype
- Create basic CMake structure
- Convert one component (maiden-repl) completely
- Document build patterns

### 5.2 Phase 2: Component Migration
- Migrate matron, crone, and other components
- Implement unit test framework

### 5.3 Phase 3: Integration
- Ensure all components build together
- Create convenience scripts for common operations

### 5.4 Phase 4: Documentation & Cleanup
- Update all build documentation
- Create examples for common development tasks

## 6. Compatibility and Risk Management

### 6.1 Backward Compatibility

During the transition period:
- Maintain WAF scripts alongside CMake
- Create a compatibility script that calls appropriate build system
- Update CI to test both systems until transition is complete

### 6.2 Specific Component Requirements

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

| Task ID  | Description                                         | Dependencies                 | Assigned To | Status      | Due Date | Notes                                                                                                              |
| -------- | --------------------------------------------------- | ---------------------------- | ----------- | ----------- | -------- | ------------------------------------------------------------------------------------------------------------------ |
| TASK-001 | Create top-level CMakeLists.txt                     | None                         |             | Completed   |          | Created initial CMakeLists.txt with component structure and build options                                          |
| TASK-002 | Ensure testing framework setup functional           | TASK-001                     |             | Completed   |          | Integrated Unity test framework and created example test                                                           |
| TASK-003 | Create platform-specific configurations             | TASK-001                     |             | Completed   |          | Created toolchain files and platform configurations for DESKTOP, PI3, and CM3                                      |
| TASK-004 | Convert matron to CMake                             | TASK-001                     |             | Completed   |          | Created CMake build system for matron. Implemented tests for HAL and event system with proper mock implementations |
| TASK-005 | Convert maiden-repl component to CMake if necessary | TASK-001, TASK-003           |             | Completed   |          | Enhanced existing maiden-repl CMake configuration, standardized build setup, and added proper test infrastructure  |
| TASK-006 | Convert crone component to CMake                    | TASK-001, TASK-003           |             | Not Started |          |                                                                                                                    |
| TASK-007 | Make softcut an independent library                 | TASK-001                     |             | Not Started |          |                                                                                                                    |
| TASK-008 | Standardize SuperCollider extension integration     | TASK-006                     |             | Not Started |          |                                                                                                                    |
| TASK-009 | Create compatibility script                         | TASK-005, TASK-006           |             | Not Started |          |                                                                                                                    |
| TASK-010 | Update CI pipeline for CMake builds                 | TASK-005, TASK-006, TASK-007 |             | Not Started |          |                                                                                                                    |
| TASK-011 | Update build documentation                          | All tasks                    |             | Not Started |          |                                                                                                                    |

### 10.3 Weekly Status Updates

**Week of April 6, 2025**
- Progress summary: Created the top-level CMakeLists.txt file that integrates the existing component-level build systems.
- Completed tasks: TASK-001
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-002 to set up the testing framework

**Week of April 7, 2025**
- Progress summary: Set up the Unity testing framework with an example test that successfully builds and runs. Created platform-specific configurations using toolchain files for DESKTOP, PI3, and CM3 platforms. Enhanced the maiden-repl CMake configuration by standardizing its build setup and adding proper test infrastructure.
- Completed tasks: TASK-002, TASK-003, TASK-005
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-006 to convert crone to CMake

**Week of April 14, 2025**
- Progress summary: Successfully converted matron to CMake including comprehensive testing capabilities. Created separate test runners for HAL and event system tests. Implemented proper mocking for HAL tests. Added detailed documentation for running tests.
- Completed tasks: TASK-004
- Tasks started: None
- Blockers encountered: None
- Next steps: Proceed with TASK-006 to convert the crone component to CMake

**Week of [Date]**
- Progress summary:
- Completed tasks:
- Tasks started:
- Blockers encountered:
- Next steps: