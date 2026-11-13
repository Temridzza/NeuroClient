# NeuroClient

**NeuroClient** is a desktop application for interacting with large language models (LLMs) through OpenAI-compatible API endpoints. It provides a chat interface, conversation history, configurable providers and models, and the ability to include local project files in the conversation context.

The application is built with **C++20, Qt 6 and QML**.

## Features

- **Streaming responses** — receive generated text incrementally in real time.
- **Conversation management** — create, rename, delete and reopen conversations.
- **Conversation branching** — edit messages, regenerate responses and switch between alternative branches.
- **Provider management** — configure API endpoints, API keys and system prompts.
- **Model management** — add and configure models associated with providers.
- **Project context** — browse a local project directory, select files and include their contents in the prompt.
- **Persistent storage** — keep conversations, messages, providers and model settings in a local SQLite database.
- **Custom system prompts** — configure prompts at the provider, model and conversation levels.

## Technology Stack

| Technology | Purpose |
|---|---|
| C++20 | Application logic |
| Qt 6 | Application framework |
| Qt Quick / QML | User interface |
| SQLite | Local data storage |
| libcurl | HTTP requests and streaming |
| nlohmann/json | JSON serialization and parsing |
| CMake | Build system |
| GoogleTest | Unit testing |

## Architecture

The application follows a layered architecture that separates the user interface, business logic, services, data access and network communication.

```text
QML Interface
     |
     v
ViewModels
     |
     v
Core / Orchestrators
     |
     +---------- Services
     |               |
     |               +---- Repositories
     |               |          |
     |               |          v
     |               |        SQLite
     |               |
     |               +---- Context / Prompt Builder
     |
     v
LLMClient
     |
     v
OpenAI-compatible API
```

### Main Components

- **ViewModels** — expose application data and actions to QML.
- **ChatOrchestrator** — coordinates message handling, conversation branching and response generation.
- **Services** — implement application operations and coordinate data access.
- **Repositories** — encapsulate SQLite queries and persistence.
- **ChatContextService** — collects conversation history, prompts and project context.
- **PromptBuilder** — assembles the messages sent to the language model.
- **LLMClient** — sends HTTP requests and processes streaming responses.

## API Compatibility

NeuroClient uses the OpenAI Chat Completions request format with streaming enabled.

It can work with services that implement a compatible API, including OpenRouter and locally hosted endpoints such as LM Studio when configured with the appropriate endpoint and model.

## Data Storage

Application data is stored locally in an SQLite database.

The database manages:

- Providers
- Models
- Conversations
- Messages and their parent-child relationships

Message relationships allow the application to preserve alternative conversation branches and track the currently selected branch.

## Building

### Requirements

- C++20-compatible compiler
- CMake
- Qt 6
- libcurl
- nlohmann/json
- cmark-gfm
- GoogleTest

### Build

```bash
git clone https://github.com/Temridzza/NeuroClient.git
cd NeuroClient

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The project also includes a `shell.nix` development environment for Nix-based systems.

## Testing

The project includes unit tests written with GoogleTest. The test suite covers selected application components, including chat orchestration, project context handling, project tree models and database repositories.

## Project Background

NeuroClient was developed as a diploma project for the specialty **09.02.07 — Information Systems and Programming**.

**Project topic:** Design and development of a desktop application for interacting with neural network models.

The project focuses on combining a desktop chat interface with configurable LLM providers, persistent conversation storage and local project context.
