# cpythonPolygon

Репозиторий с результатами экспериментов.

Текущая реализация находится в polygon3.py

В polygon1.py и polygon2.py описаны встречанные проблемы



---

## Requirements

- Python 3.10+
- Poetry 1.8.0+

## Installation

Clone the repository:

```bash
git clone https://github.com/K0tB0ris/cpythonPolygon.git
```

Install dependencies:

```bash
poetry install
```

## Development

Install requirements

```bash
poetry install --with dev
```

## Build

Before running the main program, you need to compile the c library.
A build_extension.py has been written for this.

There are two ways to launch the program.

The first is launch using poetry

```bash
poetry build
```

The second is manually (not recommended)

```bash
python3 build_extension.py
```

To remove the compiled library you can write

```bash
rm -r dist */_c*
```

## Run

To run main program
```bash
poetry run python polygon3.py
```

## License

This project is licensed under the terms of the **MIT** license. See the [LICENSE](LICENSE) for more information.
