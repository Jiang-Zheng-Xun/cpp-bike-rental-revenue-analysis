from pathlib import Path
from tempfile import TemporaryDirectory
import subprocess
import unittest


ROOT = Path(__file__).resolve().parents[1]
EXAMPLE = ROOT / "examples" / "discount_rounding" / "test_case"


class BikeRentalRegressionTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.build_dir = TemporaryDirectory()
        cls.program = Path(cls.build_dir.name) / "bike_rental"
        subprocess.run(
            [
                "g++", "-std=c++11", "-Wall", "-Wextra", "-Wpedantic",
                str(ROOT / "src" / "bike_rental.cpp"),
                "-o", str(cls.program),
            ],
            check=True,
            timeout=60,
        )

    @classmethod
    def tearDownClass(cls):
        cls.build_dir.cleanup()

    def run_case(self, inputs):
        with TemporaryDirectory() as directory:
            work = Path(directory)
            test_case = work / "test_case"
            test_case.mkdir()

            for name, content in inputs.items():
                (test_case / name).write_text(content, encoding="utf-8")

            subprocess.run(
                [str(self.program)],
                cwd=work,
                check=True,
                timeout=60,
                capture_output=True,
                text=True,
            )
            return {
                name: (work / name).read_text(encoding="utf-8").splitlines()
                for name in (
                    "part1_response.txt", "part1_status.txt",
                    "part2_response.txt", "part2_status.txt",
                )
            }

    def test_discount_response_and_per_minute_rounding(self):
        inputs = {
            name: (EXAMPLE / name).read_text(encoding="utf-8")
            for name in ("map.txt", "station.txt", "fee.txt", "user.txt")
        }
        output = self.run_case(inputs)

        self.assertEqual(
            output["part1_response.txt"],
            ["rent 1 road 00001 0", "reject", "return 2 00001 3"],
        )
        self.assertEqual(output["part1_status.txt"][-1], "0")
        self.assertEqual(
            output["part2_response.txt"],
            [
                "rent 1 road 00001 0",
                "discount electric",
                "return 2 00001 3",
            ],
        )
        self.assertEqual(output["part2_status.txt"][-1], "18")

    def test_transfer_arrival_and_same_time_request_order(self):
        inputs = {
            "map.txt": "1 2 10\n",
            "station.txt": "1 0 0 0\n2 2 0 0\n",
            "fee.txt": (
                "electric 30 40\nlady 25 35\nroad 15 25\n"
                "5\n0.8\n6\n"
            ),
            "user.txt": (
                "rent 1 electric 00001 9\n"
                "rent 1 electric 00002 10\n"
                "rent 1 electric 00003 10\n"
                "return 2 00001 19\n"
                "return 2 00002 20\n"
                "return 2 00003 20\n"
            ),
        }
        output = self.run_case(inputs)

        self.assertEqual(
            output["part2_response.txt"],
            [
                "transfer 2 1 electric 1 0",
                "rent 1 electric 00001 9",
                "reject",
                "rent 1 electric 00002 10",
                "accept",
                "rent 1 electric 00003 10",
                "reject",
                "return 2 00001 19",
                "return 2 00002 20",
                "return 2 00003 20",
            ],
        )
        self.assertEqual(output["part2_status.txt"][-1], "240")


if __name__ == "__main__":
    unittest.main()
