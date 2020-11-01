import awtk_locator as locator

def update_res(ARGUMENTS):
    locator.init(ARGUMENTS)

    import update_res_app as updater
    updater.run(locator.getAwtkRoot())

ARGUMENTS = dict()
ARGUMENTS['AWTK_ROOT'] = ''
update_res(ARGUMENTS)
