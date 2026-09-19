from __future__ import annotations

from pathlib import Path

import typer

from .data_loader import prepare_raw_data

app = typer.Typer(
    help="CLI commands for the Arduino Smart Bottle data pipeline.",
    add_completion=False,
    no_args_is_help=True,
)


@app.callback()
def main() -> None:
    """Root command for the Smart Bottle model CLI."""


@app.command("prepare-data")
def prepare_data(
    input_dir: Path = typer.Option(
        Path("model/data/raw"),
        "--input-dir",
        "-i",
        help="Directory containing raw TXT sensor exports.",
    ),
    output_dir: Path = typer.Option(
        Path("model/data/prepared"),
        "--output-dir",
        "-o",
        help="Directory where parquet files will be written.",
    ),
    force: bool = typer.Option(
        False,
        "--force",
        help="Overwrite existing parquet files in the output directory.",
    ),
) -> None:
    """Convert raw TXT samples into prepared parquet datasets."""
    prepare_raw_data(input_dir=input_dir, output_dir=output_dir, force=force)
    typer.echo(f"Prepared data written to {output_dir}")


if __name__ == "__main__":
    app()
