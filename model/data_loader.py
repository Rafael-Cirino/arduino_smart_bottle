import polars as pl
from pathlib import Path

def read_txt(file):
    df = pl.read_csv(
        file,
        new_columns=[
            "raw_t_ms",
            "acc_x",
            "acc_y",
            "acc_z",
            "gyro_x",
            "gyro_y",
            "gyro_z",
        ],
        has_header=False,
    )

    # Drop columns with all null values
    df = df[[s.name for s in df if not (s.null_count() == df.height)]]
    return df

def compute_acc_norm(df):
    return df.with_columns(pl.sum_horizontal(pl.col(r"^acc_.*$").pow(2)).sqrt().alias("acc_norm"))


def prepare_raw_data(PREPARED_PATH: Path, RAW_PATH: Path):
    PREPARED_PATH.mkdir(exist_ok=True, parents=True)
    for file in RAW_PATH.glob("*ACC*"):
        df = read_txt(file)
        df = df.with_columns(
            pl.col("raw_t_ms").cum_sum().alias("t_ms"),
            pl.col(r"^(acc|gyro)_.*$").truediv(100),
        )

        df = compute_acc_norm(df)

        df = df.select([pl.col('^.*t_ms$'), pl.col(r"^(acc|gyro)_.*$")])
        df.write_parquet(PREPARED_PATH / (file.stem.lower() + ".parquet"))