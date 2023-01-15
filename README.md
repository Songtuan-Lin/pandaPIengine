# Introduction

This is the implementation of the CYK-based total order (TO) HTN plan verification approach, which is described in the paper, *On Total-Order HTN Plan Verification with Method Preconditions – An Extension of the CYK Parsing Algorithm*, accepted by AAAI 2023. The approach is built upon the HTN planning system [pandaPI](https://panda-planner-dev.github.io/). Concretely, it relies on the data structure in pandaPI used to store an input HTN planning problem.

# Compiling the Code

Since the code is built upon pandaPI, all components required for compiling pandaPI are also required here, which are zip and gengetopt.

You can compile the code via the following commands, assuming that you are in the root directory of the code, i.e., the directory that contains this README file. 
```
mkdir build
cd build/
cmake ../src/
make
```
After executing the above commands, you can find the executable for the TOHTN plan verifier, run_verifier, in the directory build/verifier.

# Running the Plan Verifier

The executable run_verifier takes two command-line arguments, -h and -p. The argument -h specifies the path to the file containing the input HTN planning problem, and the argument -p specifies the path to the file containing the plan to be verified.
For instance, assuming that you are in the directory build, i.e., the directory where the code is compiled, the following code runs the plan verifier to check whether the plan written in the file plan.txt is a solution to the TOHTN planning problem written in the file problem.sas. In particular, we assume that the files, plan.txt and problem.sas, are also in the directory build.
```
verifier/run_verifier -h problem.sas -p plan.txt
```
Notably, since the TOHTN plan verifier can only deal with grounded problems, the file passed to the -h argument **must** be obtained by calling [pandaPIgrounder](https://github.com/panda-planner-dev/pandaPIgrounder.git), which requires an input file produced by [pandaPIparser](https://github.com/panda-planner-dev/pandaPIparser.git). In particular, when calling [pandaPIgrounder](https://github.com/panda-planner-dev/pandaPIgrounder.git), the command-line argument -t **must** be given, which transforms the input TOHTN planning problem into 2-Normal Form. For more details about how to compile and use [pandaPIparser](https://github.com/panda-planner-dev/pandaPIparser.git) and [pandaPIgrounder](https://github.com/panda-planner-dev/pandaPIgrounder.git), see the README files in the respective git repositories. The following commands produce the file problem.sas by calling the parser and the grounder, assuming that the executables pandaPIparser and pandaPIgrounder are in the directory buil, and the files domain.hddl and problem.hddl are also in the build directory.
```
./pandaPIparser domain.hddl problem.hddl problem.htn
./pandaPIgrounder -D -t problem.htn problem.sas
```
Further, the plan written in the file plan.txt should be in the format:
> action_1;action_2;action_3;...;action_n

i.e., all actions are written in a line, separated by ";".

# Experiment Results
The following table shows the experiment results where the CYK-based TOHTN plan verifier was compared against the [parsing-based][1] verifier, the [planning-based][2] verifier, and the [SAT-based][3] verifier.

The experiments were run on the benchmark set from the IPC 2020 on HTN planning, which can be obtained via the following commands:

```
mkdir ipc-benchmarks
cd ipc-benchmarks 
git clone https://github.com/panda-planner-dev/ipc-2020-plans.git
git clone https://github.com/panda-planner-dev/ipc2020-domains.git
git clone https://github.com/panda-planner-dev/domains.git
```

[1]: <https://bercher.net/publications/2021/Bartak2021TOVerification.pdf> "On the Verification of Totally-Ordered HTN Plans. Roman Barták; Simona Ondrčková; Gregor Behnke; and Pascal Bercher. In Proceedings of the 33rd IEEE International Conference on Tools with Artificial Intelligence (ICTAI 2021), pages 263–267, 2021. IEEE"

[2]: <https://bercher.net/publications/2022/Hoeller2022VerificationViaCompilation.pdf> "Compiling HTN Plan Verification Problems into HTN Planning Problems. Daniel Höller; Julia Wichlacz; Pascal Bercher; and Gregor Behnke. In Proceedings of the 32nd International Conference on Automated Planning and Scheduling (ICAPS 2022), pages 145–150, 2022. AAAI Press"

[3]: <https://staff.fnwi.uva.nl/g.behnke/papers/Behnke17Verify.pdf> "This is a solution! (... but is it though?) -- Verifying solutions of hierarchical planning problems
In Proceedings of the 27th International Conference on Automated Planning and Scheduling (ICAPS 2017), pages 20-28, 2017"