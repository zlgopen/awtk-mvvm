import awtk_locator as locator
from app_helper_base import AppHelperBase


class Helper(AppHelperBase):
    def getAwtkRoot(self):
        return locator.getAwtkOrAwtkLinuxFbRoot(self.LINUX_FB)
