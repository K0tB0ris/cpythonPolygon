# cpythonPolygon

Репозиторий с результатами экспериментов.

Текущая реализация находится в polygon3.py

В polygon1.py и polygon2.py описаны встреченные проблемы

update

В polygon4.py реализован хендер, в котором достается по одному элементу из итератора и применяем к нему операцию

В polygon5.py реализован хендер, в котором достается несколько элементов (5) из итератора, применяется к ним операция,
записывается в список из которого потом достаются значения при помощи next, когда список становится пустым, заново
достаём несколько элементов из итератора

В polygon6.py реализован хендер, в котором применение операции игнорирует все промежуточные итераторы. То есть достаем
данные из первого элемента в pipeline, дальше применяем последовательно все операции хенделеров в pipeline, и выдаем
итоговый результат.

В polygon6.py на данный момент способ применения последовательных операций реализован рекурсивно.



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
