# Lexical_Analyzer

## Overview

This project implements a **Lexical Analyzer** for the **PL/0 programming language**

The lexer reads a PL/o source fie and coverts it into a sequence of **tokens**, identifying:

- Reserved Keywords
- Identifiers
- Numbers
- Operators and symbols

It also detects lexical errors such as:
  - Identifiers longer than **11 characters**
  - Numbers longer than **5 digits**
  - Invalid characters

The program is written entirely in **C**

---

  ## Features

Supports all **PL/0 reserved words**
  - begin
  - end
  - if
  - while
  - call
  - etc.

Recognizes **identifiers** and **numbers**

Detects **invalid characters**

Identifies lexical errors for length constraints

Handles **comments** by ignoring them

Generates both **lexeme tables** and **token lists**
