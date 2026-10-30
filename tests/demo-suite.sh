#!/bin/bash
# Runs every CHASE demo; prints PASS/FAIL per check. Arg 1: demos dir.
D=${1:-/opt/chase/demos}; W=$(mktemp -d); fail=0
check() { if (eval "$2") >$W/out 2>&1; then echo "PASS  $1"; else echo "FAIL  $1"; tail -8 $W/out | sed 's/^/      /'; fail=1; fi; }
cd $D
check "DATE21 unrealizable"   "dsl_tool -i DATE21/specs/template_0_unrealizable.txt -o $W/u -b gr1c && gr1c -r $W/u.spc | grep -qx 'Not realizable.'"
check "DATE21 synthesizable"  "dsl_tool -i DATE21/specs/template_0_synthesizable.txt -o $W/s -b gr1c && gr1c -r $W/s.spc | grep -qx 'Realizable.' && gr1c -t dot $W/s.spc > $W/s.dot"
check "DATE18 realizable"     "dsl_tool -i DATE18/template_0_realizable.txt -o $W/r -b gr1c && gr1c -r $W/r.spc | grep -qx 'Realizable.'"
check "DATE18 unrealizable"   "dsl_tool -i DATE18/template_0_unrealizable.txt -o $W/n -b gr1c && gr1c -r $W/n.spc | grep -qx 'Not realizable.'"
check "DATE18 slugs"          "dsl_tool -i DATE18/template_0_realizable.txt -o $W/sl && python3 $SLUGSCOMPILER $W/sl.structuredSlugs > $W/sl.in && slugs $W/sl.in 2>&1 | grep -q 'is realizable'"
check "FDL2024 realizable"    "dsl_tool -i FDL2024/dsl_factory_realizable.txt -o $W/f -b gr1c && gr1c -r $W/f.spc | grep -qx 'Realizable.'"
check "DATE21 logics script"  "cd DATE21 && logics_tool -i specs/robotic_arm.ltl -o $W/d21 -c solution.script && cd .. && test -s $W/d21/TM.smv -a -s $W/d21/MT.smv -a -s $W/d21/system.smv && python3 $SLUGSCOMPILER $W/d21/coordinator.structuredSlugs > $W/d21/c.in && slugs $W/d21/c.in 2>&1 | grep -q 'is realizable'"
check "RoboticJoints verify"  "cd RoboticJoints && logics_tool -i specs.ltl -o $W/rj -c verification.chase && cd .. && test -s $W/rj/refinement.smv"
check "RoboticJoints synth"   "cd RoboticJoints && logics_tool -i specs.ltl -o $W/rj -c synthesis.chase && cd .. && for k in 1 2 3; do python3 $SLUGSCOMPILER $W/rj/joint\$k.structuredSlugs > $W/rj/j\$k.in && slugs $W/rj/j\$k.in 2>&1 | grep -q 'is realizable' || exit 1; done"
check "CPSWeek22 verify"      "cd CPSWeek22 && logics_tool -i specs.ltl -o $W/c -c verification.chase && cd .. && test -s $W/c/refinement.smv"
check "CPSWeek22 single model" "cd CPSWeek22 && logics_tool -i single_model.ltl -o $W/c -c singlemodel.chase && cd .. && test -s $W/c/single_model.structuredSlugs"
printf 'NAME: x;\nCONTRACT A:\n Assumptions: []p;\n' > $W/bad.ltl
check "syntax error reported" "logics_tool -i $W/bad.ltl -o $W/b -c /dev/null 2>&1 | grep -q 'Syntax errors'"
printf 'NAME: shared;\ninteger variable n;\nreal variable v;\nreal constant L 2.5;\n\nCONTRACT A:\n    output boolean variable w;\n    Assumptions:\n        true;\n    Guarantees:\n        G((n > 4) -> X(w));\n        G((v > L) -> X(w));\n' > $W/shared.ltl
printf 'refinement A A r.smv;\n' > $W/r.chase
check "shared declarations, reals" "logics_tool -i $W/shared.ltl -o $W/sh -c $W/r.chase && grep -q 'DEFINE L := 2.5;' $W/sh/r.smv && grep -q 'v : real;' $W/sh/r.smv && grep -q 'n : 0..16;' $W/sh/r.smv"
NUXMV=$(command -v nuXmv || ls ~/.chase/bin/nuXmv 2>/dev/null)
check "reals: nuXmv hint"     "logics_tool -i $W/shared.ltl -o $W/sh -c $W/r.chase 2>&1 | grep -q 'check it with nuXmv' && head -1 $W/sh/r.smv | grep -q 'NuSMV cannot read'"
if [ -n "$NUXMV" ]; then
    printf 'set on_failure_script_quits\nread_model -i %s\ngo_msat\ncheck_ltlspec_ic3\nquit\n' $W/sh/r.smv > $W/r.cmd
    check "reals: nuXmv go_msat" "test \$($NUXMV -source $W/r.cmd </dev/null | grep -c 'is true') = 2"
    # nuXmv on a file; prints true or false for each LTLSPEC
    verdicts() { printf 'set on_failure_script_quits\nread_model -i %s\n%s\ncheck_ltlspec_ic3\nquit\n' "$1" "$2" > $W/v.cmd
                 $NUXMV -source $W/v.cmd </dev/null | grep -oE 'is (true|false)$' | cut -c4- | tr '\n' ' '; }
    charger_console() {
        cd Charger && logics_tool -i charger.ltl -o $W/chg -c charger.chase && cd .. || return 1
        python3 $SLUGSCOMPILER $W/chg/greedy.structuredSlugs > $W/chg/greedy.in &&
        slugs $W/chg/greedy.in 2>&1 | grep -q 'is unrealizable' &&
        python3 $SLUGSCOMPILER $W/chg/fair.structuredSlugs > $W/chg/fair.in &&
        slugs $W/chg/fair.in 2>&1 | grep -q 'is realizable' &&
        test "$(verdicts $W/chg/refinement.smv 'go
build_boolean_model')" = "true true "
    }
    lpc_console() {
        cd LiftPlusCruise && logics_tool -i lpc.ltl -o $W/lpc -c lpc.chase && cd .. || return 1
        test "$(verdicts $W/lpc/vehicle12.smv go_msat)" = "false false false " &&
        test "$(verdicts $W/lpc/vehicle06.smv go_msat)" = "false true true "
    }
    check "Charger"               "python3 Charger/charger.py --expect"
    check "Charger console"       "charger_console"
    check "LiftPlusCruise"        "python3 LiftPlusCruise/lpc.py --expect"
    check "LiftPlusCruise console" "lpc_console"
    check "LMFSM"                 "python3 LMFSM/fsm.py --expect"
    check "AHB"                   "python3 AHB/ahb.py --expect"
else
    echo "SKIP  reals: nuXmv go_msat, Charger, LiftPlusCruise, LMFSM, AHB (no nuXmv)"
fi
# NuSMV, as without nuXmv (the image published on ghcr.io); BufferSwap needs Yosys
if command -v NuSMV >/dev/null; then
    for d in Charger/charger.py LMFSM/fsm.py AHB/ahb.py; do
        check "NuSMV: $d" "CHASE_BACKEND=nusmv python3 $d --expect"
    done
fi
if command -v yosys >/dev/null || command -v yowasp-yosys >/dev/null; then
    check "BufferSwap"            "python3 BufferSwap/bufferswap.py --expect"
fi
check "command line"          "chase-cps --version"
check "python modules"        "python3 -c 'import pychase, pychase_backends, pychase_DSL, pychase_logicsLang'"
exit $fail
