#!/bin/bash
# Runs every CHASE demo; prints PASS/FAIL per check. Arg 1: demos dir.
D=${1:-/opt/chase/demos}; W=$(mktemp -d); fail=0
check() { if (eval "$2") >/dev/null 2>&1; then echo "PASS  $1"; else echo "FAIL  $1"; fail=1; fi; }
cd $D
check "DATE21 unrealizable"   "dsl_tool -i DATE21/specs/template_0_unrealizable.txt -o $W/u -b gr1c && gr1c -r $W/u.spc | grep -qx 'Not realizable.'"
check "DATE21 synthesizable"  "dsl_tool -i DATE21/specs/template_0_synthesizable.txt -o $W/s -b gr1c && gr1c -r $W/s.spc | grep -qx 'Realizable.' && gr1c -t dot $W/s.spc > $W/s.dot"
check "DATE18 realizable"     "dsl_tool -i DATE18/template_0_realizable.txt -o $W/r -b gr1c && gr1c -r $W/r.spc | grep -qx 'Realizable.'"
check "DATE18 unrealizable"   "dsl_tool -i DATE18/template_0_unrealizable.txt -o $W/n -b gr1c && gr1c -r $W/n.spc | grep -qx 'Not realizable.'"
check "DATE18 slugs"          "dsl_tool -i DATE18/template_0_realizable.txt -o $W/sl && python3 $SLUGSCOMPILER $W/sl.structuredSlugs > $W/sl.in && slugs $W/sl.in 2>&1 | grep -q 'is realizable'"
check "FDL2024 realizable"    "dsl_tool -i FDL2024/dsl_factory_realizable.txt -o $W/f -b gr1c && gr1c -r $W/f.spc | grep -qx 'Realizable.'"
check "DATE21 logics script"  "cd DATE21 && logics_tool -i specs/robotic_arm.ltl -o $W/d21 -c solution.script && cd .. && test -s $W/d21/TM.smv -a -s $W/d21/MT.smv -a -s $W/d21/system.smv && gr1c -r $W/d21/coordinator.spc | grep -qx 'Realizable.'"
check "RoboticJoints verify"  "cd RoboticJoints && logics_tool -i specs.ltl -o $W/rj -c verification.chase && cd .. && test -s $W/rj/refinement.smv"
check "RoboticJoints synth"   "cd RoboticJoints && logics_tool -i specs.ltl -o $W/rj -c synthesis.chase && cd .. && for k in 1 2 3; do python3 $SLUGSCOMPILER $W/rj/joint\$k.structuredSlugs > $W/rj/j\$k.in && slugs $W/rj/j\$k.in 2>&1 | grep -q 'is realizable' || exit 1; done"
check "CPSWeek22 verify"      "cd CPSWeek22 && logics_tool -i specs.ltl -o $W/c -c verification.chase && cd .. && test -s $W/c/refinement.smv"
check "CPSWeek22 single model" "cd CPSWeek22 && logics_tool -i single_model.ltl -o $W/c -c singlemodel.chase && cd .. && test -s $W/c/single_model.structuredSlugs"
printf 'NAME: x;\nCONTRACT A:\n Assumptions: []p;\n' > $W/bad.ltl
check "syntax error reported" "logics_tool -i $W/bad.ltl -o $W/b -c /dev/null 2>&1 | grep -q 'Syntax errors'"
check "python modules"        "python3 -c 'import pychase, pychase_backends, pychase_DSL, pychase_logicsLang'"
exit $fail
