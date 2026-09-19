# Smart Bottle model workflow

This folder contains the data preparation and modeling pipeline for the Arduino Smart Bottle project.

## Structure

- `data/raw`: original TXT sensor exports from the Arduino logger.
- `data/prepared`: normalized Parquet outputs ready for analysis.
- `data_loader.py`: reusable dataset preparation logic.
- `cli.py`: Typer command-line interface.
- `notebooks`: exploratory analysis notebooks.

## Quick start

From the repository root:

```bash
uv sync
uv run smart-bottle-model prepare-data
```

You can also point the data directory explicitly:

```bash
uv run smart-bottle-model prepare-data --input-dir model/data/raw --output-dir model/data/prepared
```

## What the pipeline does

1. Reads raw TXT files from the sensor dataset.
2. Normalizes accelerometer and gyroscope values.
3. Computes the acceleration magnitude.
4. Writes one Parquet file per source file to `data/prepared`.

This keeps the raw captures untouched while creating a consistent dataset for training and analysis.
