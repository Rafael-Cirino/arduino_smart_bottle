# Arduino Smart Bottle

## English

The Arduino Smart Bottle is a low-cost hydration-monitoring prototype that combines an accelerometer, an Arduino Uno, and a microSD logger to detect drinking events and record sensor data. The project is designed to support habit tracking, pattern analysis, and experiments in embedded health sensing.

### Smart Bottle article summary

The article presents the Smart Bottle as a practical low-cost embedded system for monitoring hydration behavior in real time. By measuring acceleration patterns associated with bottle use, the device identifies drinking events and records them in a structured dataset. The project combines hardware sensing, firmware logging, and data analysis to turn a simple bottle into a tool for habit tracking, health awareness, and early experimentation with wearable-like IoT behavior.

### Repository overview

This repository contains both the embedded firmware and the Python workflow used to inspect and prepare the recorded sensor data.

```text
arduino_smart_bottle/
├── README.md
├── LICENSE
├── .gitignore
├── data_logger/
│   └── data_logger.ino
├── smart_bottle/
│   └── smart_bottle.ino
├── timer_interrupt/
│   └── timer_interrupt.ino
├── model/
│   ├── README.md
│   ├── __init__.py
│   ├── __main__.py
│   ├── cli.py
│   ├── data_loader.py
│   ├── data/
│   │   ├── raw/
│   │   ├── prepared/
│   │   └── test_annotations/
│   ├── data_exploration.ipynb
│   ├── model_train.ipynb
│   ├── pyproject.toml
│   └── uv.lock
├── tests/
│   └── test_cli.py
├── pyproject.toml
├── smart bottle article.pdf
└── plan.md
```

### Main components

- `data_logger/`: Arduino firmware for the main data-logging flow.
- `smart_bottle/`: project firmware for the smart bottle device.
- `timer_interrupt/`: interrupt-driven timing experiments.
- `model/`: Python data preparation and analysis pipeline.
- `tests/`: automated checks for the command-line workflow.

### Environment setup

This repository uses `uv` as the standard environment and dependency manager.

```bash
uv sync --extra dev
```

To run the model workflow from the repository root:

```bash
uv run smart-bottle-model prepare-data
```

To target custom input/output folders:

```bash
uv run smart-bottle-model prepare-data --input-dir model/data/raw --output-dir model/data/prepared
```

### Data pipeline

The Python workflow in `model/` reads raw TXT sensor exports from `model/data/raw`, normalizes the accelerometer and gyroscope values, computes the acceleration magnitude, and writes one Parquet file per raw source file into `model/data/prepared`.

The typical flow is:

```text
raw TXT samples
    ↓
read_txt()
    ↓
normalize values
    ↓
compute_acc_norm()
    ↓
write Parquet dataset
```

### CLI

The canonical Typer entry point is:

```bash
uv run smart-bottle-model --help
```

Available command:

```bash
uv run smart-bottle-model prepare-data --help
```

### Testing

```bash
uv run pytest
```

### Notes

- The raw dataset is intentionally preserved under `model/data/raw`.
- Prepared data is generated under `model/data/prepared`.
- Exploratory notebooks remain available for analysis, while the reusable workflow is exposed through the CLI.

---

## Português

O Arduino Smart Bottle é um protótipo de baixo custo para monitoramento de hidratação, combinando acelerômetro, Arduino Uno e logger em microSD para detectar eventos de ingestão e registrar dados de sensores. O projeto foi pensado para apoiar rastreamento de hábitos, análise de padrões e experimentos em sensoriamento embarcado para saúde.

### Resumo do artigo do Smart Bottle

O artigo apresenta a garrafa inteligente como um sistema embarcado prático e de baixo custo para monitorar o comportamento de hidratação em tempo real. Ao medir padrões de aceleração associados ao uso da garrafa, o dispositivo identifica eventos de ingestão e os registra em um conjunto de dados estruturado. O projeto combina sensoriamento em hardware, registro em firmware e análise de dados para transformar uma garrafa simples em uma ferramenta para rastreamento de hábitos, conscientização sobre saúde e experimentação inicial com comportamento IoT semelhante ao de dispositivos vestíveis.

### Visão geral do repositório

Este repositório contém tanto o firmware embarcado quanto o fluxo em Python usado para inspecionar e preparar os dados gravados pelos sensores.

```text
arduino_smart_bottle/
├── README.md
├── LICENSE
├── .gitignore
├── data_logger/
│   └── data_logger.ino
├── smart_bottle/
│   └── smart_bottle.ino
├── timer_interrupt/
│   └── timer_interrupt.ino
├── model/
│   ├── README.md
│   ├── __init__.py
│   ├── __main__.py
│   ├── cli.py
│   ├── data_loader.py
│   ├── data/
│   │   ├── raw/
│   │   ├── prepared/
│   │   └── test_annotations/
│   ├── data_exploration.ipynb
│   ├── model_train.ipynb
│   ├── pyproject.toml
│   └── uv.lock
├── tests/
│   └── test_cli.py
├── pyproject.toml
├── smart bottle article.pdf
└── plan.md
```

### Principais componentes

- `data_logger/`: firmware principal do sistema de coleta de dados em Arduino.
- `smart_bottle/`: firmware do protótipo da garrafa inteligente.
- `timer_interrupt/`: testes de temporização com interrupções.
- `model/`: pipeline em Python para preparação e análise dos dados.
- `tests/`: validações automatizadas do fluxo de linha de comando.

### Configuração do ambiente

Este repositório usa `uv` como gerenciador padrão de ambiente e dependências.

```bash
uv sync --extra dev
```

Para executar o fluxo do modelo a partir da raiz do repositório:

```bash
uv run smart-bottle-model prepare-data
```

Para usar diretórios personalizados de entrada e saída:

```bash
uv run smart-bottle-model prepare-data --input-dir model/data/raw --output-dir model/data/prepared
```

### Pipeline de dados

O fluxo em Python em `model/` lê os arquivos TXT brutos de sensores em `model/data/raw`, normaliza os valores de acelerômetro e giroscópio, calcula a magnitude da aceleração e grava um arquivo Parquet para cada fonte bruta em `model/data/prepared`.

O fluxo típico é:

```text
amostras TXT brutas
    ↓
read_txt()
    ↓
normalização dos valores
    ↓
compute_acc_norm()
    ↓
gravação do dataset em Parquet
```

### CLI

O ponto principal de entrada via Typer é:

```bash
uv run smart-bottle-model --help
```

Comando disponível:

```bash
uv run smart-bottle-model prepare-data --help
```

### Testes

```bash
uv run pytest
```

### Observações

- Os dados brutos são preservados em `model/data/raw`.
- Os dados preparados são gerados em `model/data/prepared`.
- Os notebooks exploratórios continuam disponíveis para análise, enquanto o fluxo reutilizável fica exposto por meio da CLI.
