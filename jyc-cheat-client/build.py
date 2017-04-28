#!/Users/joyeongchan/anaconda/bin/python
import os
import sys
import stat
from optparse import OptionParser
import subprocess as sp


def main():
    work_dir = "/Users/joyeongchan/projects/jyc-cheat/jyc-cheat-client/"
    executable = "jyc_cheat_client"
    executable_args = ""

    print("---- Start to build jyc-cheat ----")
    
    os.chdir(work_dir)

    parser = OptionParser()
    parser.add_option("--debug_mode", action="store_true", default=False,
                      dest="is_debug", help="The option for building with debug or not.")
    parser.add_option("--cmd_export", action="store_true", default=False,
                      dest="is_cpl_export", help="The export of the compile command.")
    parser.add_option("--execute", action="store_true", default=False,
                      dest="is_exec_bin", help="Executing of the binary file.")
    parser.add_option("-a", "--args", action="store", type="string",
                      dest="args", help="Arguaments of executable")
    (options, args) = parser.parse_args()

    arguments = ""
    if options.is_debug == True:
        arguments += "-DCMAKE_BUILD_TYPE=Debug "

    if options.is_cpl_export == True:
        arguments += "-DCMAKE_EXPORT_COMPILE_COMMANDS=1 "


    print("-------- cmake arguments --------")
    print(arguments)
    print("---------------------------------")

    cmake_cmd = " cmake {0} .".format(arguments)
    os.system(cmake_cmd)
    os.system("make")

    print(os.getcwd())

    st = os.stat(executable)
    os.chmod(executable, st.st_mode | stat.S_IEXEC)

    if options.is_exec_bin == True:
        if options.args is not None:
            executable_args += options.args
        # print("executable_args : ", executable_args)
        # os.system("./{} {}".format(executable))
        # os.system(executable)
        proc_cmd = "{0}/{1} {2}".format(work_dir, executable, executable_args)
        print("proc_cmd : ", proc_cmd)
        os.system("make")
        os.system(proc_cmd)
        # print("executable_args : ", executable_args)

if __name__ == '__main__':
    main()
