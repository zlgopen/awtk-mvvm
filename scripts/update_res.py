import sys
import awtk_locator as locator

LONGSOPTS = ['awtk_root=', 'AWTK_ROOT=']
def get_args(args, longsopts = []) :
    list_opts = []
    for arg in args:
        if arg.startswith('--') :
            tmp_opt = '';
            for opt in longsopts:
                if arg.find(opt) > 0 :
                    tmp_opt = opt;
                    break
            if tmp_opt != '' :
                list_opts.append(arg.split(tmp_opt)[1])
                continue
            else :
                print(arg + " not find command, command :")
                print(longsopts)
                sys.exit()
    return list_opts


def update_res(ARGUMENTS, is_new_usage):
    locator.init(ARGUMENTS)

    import update_res_app as updater
    if is_new_usage and not hasattr(updater, "getopt") :
        print(" must update awtk !!!")
        sys.exit()
    updater.run(locator.getAwtkRoot())

is_new_usage = False
opts = get_args(sys.argv[1:], LONGSOPTS)
ARGUMENTS = dict()
if len(opts) > 0 :
    is_new_usage = True
    ARGUMENTS['AWTK_ROOT'] = opts[0]
else :
    ARGUMENTS['AWTK_ROOT'] = ''
update_res(ARGUMENTS, is_new_usage)
