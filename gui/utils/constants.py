import os


class Defaults:
    """
    ### Default values for the application.
    """

    LOCAL_PORT: int = 321


class Paths:
    """
    ### Paths for the application.
    """

    ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    BIN = os.path.join(ROOT, "bin")
    LIB = os.path.join(ROOT, "lib")
