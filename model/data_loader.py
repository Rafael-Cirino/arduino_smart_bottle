from __future__ import annotations

from pathlib import Path

import polars as pl

DEFAULT_RAW_PATH = Path("model/data/raw")
DEFAULT_PREPARED_PATH = Path("model/data/prepared")


def read_txt(file: Path | str):
    df = pl.read_csv(
        file,
        separator=",",
        has_header=False,
    )

    if df.width > 0 and df.get_column(df.columns[-1]).null_count() == df.height:
        df = df[:, :-1]

    expected_columns = [
        "raw_t_ms",
        "acc_x",
        "acc_y",
        "acc_z",
        "gyro_x",
        "gyro_y",
        "gyro_z",
        "drink",
    ]

    if df.width == len(expected_columns):
        df.columns = expected_columns
    elif df.width == 5:
        df.columns = ["raw_t_ms", "acc_x", "acc_y", "acc_z", "drink"]
    else:
        raise ValueError(
            f"Unsupported raw data format in {file}: expected 5 or 8 columns after trimming trailing nulls, got {df.width}"
        )

    df = df[[s.name for s in df if not (s.null_count() == df.height)]]
    return df


def compute_acc_norm(df):
    if (
        "acc_x" not in df.columns
        or "acc_y" not in df.columns
        or "acc_z" not in df.columns
    ):
        raise ValueError(
            "Input dataframe must include accelerometer columns: acc_x, acc_y, acc_z"
        )
    return df.with_columns(
        pl.sum_horizontal(pl.col(r"^acc_.*$").pow(2)).sqrt().alias("acc_norm")
    )


def prepare_raw_data(
    input_dir: Path | str = DEFAULT_RAW_PATH,
    output_dir: Path | str = DEFAULT_PREPARED_PATH,
    force: bool = False,
):
    source_dir = Path(input_dir)
    target_dir = Path(output_dir)

    if not source_dir.exists():
        raise FileNotFoundError(f"Raw data directory not found: {source_dir}")

    target_dir.mkdir(exist_ok=True, parents=True)
    written_files: list[Path] = []

    for file in sorted(source_dir.glob("*")):
        if not file.is_file():
            continue

        target = target_dir / (file.stem.lower() + ".parquet")
        if target.exists() and not force:
            continue

        df = read_txt(file)
        df = df.with_columns(
            pl.col("raw_t_ms").cum_sum().alias("t_ms"),
            pl.col(r"^(acc|gyro)_.*$").truediv(100),
        )

        df = compute_acc_norm(df)
        df = df.select(
            [pl.col("^.*t_ms$"), pl.col(r"^(acc|gyro)_.*$"), pl.col("drink")]
        )
        df.write_parquet(target)
        written_files.append(target)

    return written_files


if __name__ == "__main__":
    prepare_raw_data()
