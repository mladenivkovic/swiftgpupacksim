#!/usr/bin/env python3

import os
import numpy as np

class ResultData:
    """
    Class to read in and hold result data.

    Access tasks and timings via ResultData.tasks and ResultData.timings
    """

    verbose = True

    def __init__(self, srcfile: str, verbose=False):
        """
        Parameters
        ----------

        srcfile: str
            path to result file to read in and process
        """

        if not os.path.exists(srcfile):
            raise FileNotFoundError(f"File {srcfile} not found.")

        self.verbose = verbose
        self.srcfile = srcfile
        self.layout = self._get_layout_from_filename()
        self.tasks, self.timings, self.data_dict = self._read_results_from_file()

        return



    def _get_layout_from_filename(self):
        """
        Get the particle memory layout from the file name.
        """

        bname = os.path.basename(self.srcfile)

        if not bname.endswith(".csv"):
            raise ValueError("I expected a file with suffix '.csv'")
        if not bname.startswith("results_"):
            raise ValueError("I expected a file starting with 'results_'")

        layout = bname[len("results_"):-len(".csv")]

        if self.verbose:
            print(f"-- Found layout {layout}")

        return layout


    def _read_results_from_file(self):

        data_d = {
                "pack/density": None,
                "pack/gradient": None,
                "pack/force": None,
                "unpack/density": None,
                "unpack/gradient": None,
                "unpack/force": None,
                }

        with open(self.srcfile) as fp:
            for line in fp:
                if line.startswith("#"):
                    continue
                else:
                    task, subtask, timing_str = line.split(",")
                    data_d[task+"/"+subtask] = float(timing_str)


        tasks = np.array(["pack/density", "pack/gradient", "pack/force", "unpack/density", "unpack/gradient", "unpack/force"])
        timings = np.array([data_d["pack/density"], data_d["pack/gradient"], data_d["pack/force"], data_d["unpack/density"], data_d["unpack/gradient"], data_d["unpack/force"]])

        return tasks, timings, data_d


