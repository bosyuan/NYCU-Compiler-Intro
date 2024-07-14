#!/usr/bin/env python3

import os
import subprocess
import sys
from argparse import ArgumentParser

import colorama


class Grader:
    basic_case_dir = "./basic_cases"
    basic_cases = {
        1: "variableConstant",
        2: "expression",
        3: "function",
        4: "specExample",
        5: "condition",
        6: "loop"
    }
    basic_case_scores = [0, 10, 10, 10, 10, 10, 10]
    basic_id_list = basic_cases.keys()

    advance_case_dir = "./advance_cases"
    advance_cases = {
        1: "advCond",
        2: "advExpr",
        3: "advFunc",
        4: "advLoop1",
        5: "advLoop2",
        6: "argument",
        7: "negative"
    }
    advance_case_scores = [0, 5, 5, 5, 5, 5, 5, 5]
    advance_id_list = advance_cases.keys()

    bonus_case_dir = "./bonus_cases"
    bonus_cases = {
        1: "booleantest1",
        2: "booleantest2",
        3: "arraytest1",
        4: "arraytest2",
        5: "stringtest",
        6: "realtest1",
        7: "realtest2"
    }
    bonus_case_scores = [0, 2, 2, 3, 3, 3, 3, 3]
    bonus_id_list = bonus_cases.keys()

    diff_result = ""

    def __init__(self, compiler, save_path, executable_file_path,
                 code_result_path, io_file):
        self.compiler = compiler
        self.io_file = io_file

        self.save_path = save_path
        if not os.path.exists(self.save_path):
            os.makedirs(self.save_path)

        self.executable_file_path = executable_file_path
        if not os.path.exists(self.executable_file_path):
            os.makedirs(self.executable_file_path)

        self.code_result_path = code_result_path
        if not os.path.exists(self.code_result_path):
            os.makedirs(self.code_result_path)

        self.output_dir = "result"
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)

    def gen_riscv_code(self, case_type, case_id):
        if case_type == "basic":
            test_case = "%s/%s/%s.p" % (self.basic_case_dir,
                                        "test-cases", self.basic_cases[case_id])
        elif case_type == "advance":
            test_case = "%s/%s/%s.p" % (self.advance_case_dir,
                                        "test-cases", self.advance_cases[case_id])
        elif case_type == "bonus":
            test_case = "%s/%s/%s.p" % (self.bonus_case_dir,
                                        "test-cases", self.bonus_cases[case_id])

        clist = [self.compiler, test_case, "--save-path", self.save_path]
        try:
            proc = subprocess.Popen(
                clist, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        except Exception as e:
            print(colorama.Fore.RED + "Call of '%s' failed: %s" %
                  (" ".join(clist), e))
            exit(1)

        proc.wait()

    def compile_riscv_code(self, case_type, case_id):
        if case_type == "basic":
            test_case = "%s/%s.S" % (self.save_path, self.basic_cases[case_id])
            executable_file = "%s/%s" % (self.executable_file_path,
                                         self.basic_cases[case_id])
        elif case_type == "advance":
            test_case = "%s/%s.S" % (self.save_path,
                                     self.advance_cases[case_id])
            executable_file = "%s/%s" % (self.executable_file_path,
                                         self.advance_cases[case_id])
        elif case_type == "bonus":
            test_case = "%s/%s.S" % (self.save_path, self.bonus_cases[case_id])
            executable_file = "%s/%s" % (self.executable_file_path,
                                         self.bonus_cases[case_id])

        clist = ["riscv32-unknown-elf-gcc", test_case,
                 self.io_file, "-o", executable_file]
        try:
            proc = subprocess.Popen(
                clist, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        except Exception as e:
            print(colorama.Fore.RED + "Call of '%s' failed: %s" %
                  (" ".join(clist), e))
            exit(1)

        proc.wait()

    def run_riscv_code(self, case_type, case_id):
        if case_type == "basic":
            output_file = "%s/%s" % (self.code_result_path,
                                     self.basic_cases[case_id])
            executable_file = "%s/%s" % (self.executable_file_path,
                                         self.basic_cases[case_id])
        elif case_type == "advance":
            output_file = "%s/%s" % (self.code_result_path,
                                     self.advance_cases[case_id])
            executable_file = "%s/%s" % (self.executable_file_path,
                                         self.advance_cases[case_id])
        elif case_type == "bonus":
            output_file = "%s/%s" % (self.code_result_path,
                                     self.bonus_cases[case_id])
            executable_file = "%s/%s" % (self.executable_file_path,
                                         self.bonus_cases[case_id])

        clist = ["spike", "--isa=RV32",
                 "/risc-v/riscv32-unknown-elf/bin/pk", executable_file]
        try:
            proc = subprocess.Popen(
                clist, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            stdout_bytes, stderr_bytes = proc.communicate(input=b"123")
        except Exception as e:
            print(colorama.Fore.RED + "Call of '%s' failed: %s" %
                  (" ".join(clist), e))
            exit(1)

        proc.wait()

        stdout = stdout_bytes.decode()
        stderr = stderr_bytes.decode()
        with open(output_file, "w") as out:
            out.write(stdout)
            out.write(stderr)

    def compare_file_content(self, case_type, case_id):
        if case_type == "basic":
            output_file = "%s/%s" % (self.code_result_path,
                                     self.basic_cases[case_id])
            solution = "%s/%s/%s" % (self.basic_case_dir,
                                     "sample-solutions", self.basic_cases[case_id])
        elif case_type == "advance":
            output_file = "%s/%s" % (self.code_result_path,
                                     self.advance_cases[case_id])
            solution = "%s/%s/%s" % (self.advance_case_dir,
                                     "sample-solutions", self.advance_cases[case_id])
        elif case_type == "bonus":
            output_file = "%s/%s" % (self.code_result_path,
                                     self.bonus_cases[case_id])
            solution = "%s/%s/%s" % (self.bonus_case_dir,
                                     "sample-solutions", self.bonus_cases[case_id])

        clist = ["diff", "-Z", "-u", output_file, solution,
                 f'--label="your output:({output_file})"', f'--label="answer:({solution})"']
        try:
            proc = subprocess.Popen(
                clist, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        except Exception as e:
            print("Call of '%s' failed: %s" % (" ".join(clist), e))
            return False

        output = str(proc.stdout.read(), "utf-8")
        retcode = proc.wait()
        if retcode != 0:
            if case_type == "basic":
                self.diff_result += "{}\n".format(self.basic_cases[case_id])
            elif case_type == "advance":
                self.diff_result += "{}\n".format(self.advance_cases[case_id])
            elif case_type == "bonus":
                self.diff_result += "{}\n".format(self.bonus_cases[case_id])
            self.diff_result += "{}\n".format(output)

        return retcode == 0

    def test_sample_case(self, case_type, case_id):
        self.gen_riscv_code(case_type, case_id)
        self.compile_riscv_code(case_type, case_id)
        self.run_riscv_code(case_type, case_id)

        return self.compare_file_content(case_type, case_id)

    def run(self) -> int:
        print("---\tCase\t\tPoints")

        total_score = 0
        max_score = 0

        for b_id in self.basic_id_list:
            c_name = self.basic_cases[b_id]
            print("+++ TESTING basic case %s:" % c_name)
            ok = self.test_sample_case("basic", b_id)
            max_val = self.basic_case_scores[b_id]
            get_val = max_val if ok else 0
            self.set_text_color(ok)
            print("---\t%s\t%d/%d" % (c_name, get_val, max_val))
            self.reset_text_color()
            total_score += get_val
            max_score += max_val

        for a_id in self.advance_id_list:
            c_name = self.advance_cases[a_id]
            print("+++ TESTING advance case %s:" % c_name)
            ok = self.test_sample_case("advance", a_id)
            max_val = self.advance_case_scores[a_id]
            get_val = max_val if ok else 0
            self.set_text_color(ok)
            print("---\t%s\t%d/%d" % (c_name, get_val, max_val))
            self.reset_text_color()
            total_score += get_val
            max_score += max_val

        for b_id in self.bonus_id_list:
            c_name = self.bonus_cases[b_id]
            print("+++ TESTING bonus case %s:" % c_name)
            ok = self.test_sample_case("bonus", b_id)
            max_val = self.bonus_case_scores[b_id]
            get_val = max_val if ok else 0
            self.set_text_color(ok)
            print("---\t%s\t%d/%d" % (c_name, get_val, max_val))
            self.reset_text_color()
            total_score += get_val
            max_score += max_val

        self.set_text_color(total_score == max_score)
        print("---\tTOTAL\t\t%d/%d" % (total_score, max_score))
        self.reset_text_color()

        with open("{}/{}".format(self.output_dir, "score.txt"), "w") as result:
            result.write("---\tTOTAL\t\t%d/%d" % (total_score, max_score))

        with open("{}/{}".format(self.output_dir, "diff.txt"), 'w') as diff:
            diff.write(self.diff_result)

        # NOTE: Return 1 on test failure to support GitHub CI; otherwise, such
        # CI never fails.
        if total_score != max_score:
            return 1
        return 0

    @staticmethod
    def set_text_color(test_passed: bool) -> None:
        """Sets the color based on whether the test has passed or not."""
        if test_passed:
            color = colorama.Fore.GREEN
        else:
            color = colorama.Fore.RED
        print(color, end='')

    @staticmethod
    def reset_text_color() -> None:
        print(colorama.Style.RESET_ALL, end='')


def main() -> int:
    parser = ArgumentParser()
    parser.add_argument(
        "--compiler", help="Your compiler to test.", default="../src/compiler")
    parser.add_argument(
        "--save-path", help="Path that stores the output risc-v instructions of your compiler.", default="./output_riscv_code")
    parser.add_argument("--executable-file-path",
                        help="Path that stores the compiled executable files.", default="./executable")
    parser.add_argument(
        "--code-result-path", help="Path that stores the output content of your generated risc-v instructions.", default="./code_executed_result")
    parser.add_argument(
        "--io-file", help="IO file for io function", default="./io.c")
    args = parser.parse_args()

    g = Grader(compiler=args.compiler, save_path=args.save_path, executable_file_path=args.executable_file_path,
               code_result_path=args.code_result_path, io_file=args.io_file)
    return g.run()


if __name__ == "__main__":
    sys.exit(main())
