from pathlib import Path

from typer.testing import CliRunner

from model.cli import app


runner = CliRunner()


def test_prepare_data_creates_parquet_files(tmp_path: Path):
    raw_dir = tmp_path / "raw"
    raw_dir.mkdir()

    (raw_dir / "sample_8col.txt").write_text(
        "0,1,2,3,4,5,6,7\n100,10,20,30,40,50,60,1\n",
        encoding="utf-8",
    )
    (raw_dir / "sample_5col.txt").write_text(
        "0,1,2,3,4\n100,10,20,30,1\n",
        encoding="utf-8",
    )

    output_dir = tmp_path / "prepared"

    result = runner.invoke(
        app,
        ["prepare-data", "--input-dir", str(raw_dir), "--output-dir", str(output_dir)],
    )

    assert result.exit_code == 0, result.output
    assert output_dir.joinpath("sample_8col.parquet").exists()
    assert output_dir.joinpath("sample_5col.parquet").exists()
