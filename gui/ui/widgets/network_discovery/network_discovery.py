from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import QWidget

from controllers import NetworkController

from .handlers import NetworkDiscoveryHandler
from .ui import NetworkDiscoveryUI


class NetworkDiscovery(QWidget):
    """
    ### Widget for discovering the public network socket.

    #### Attributes:
    - `controller (NetworkController)`: The network controller.
    - `stun_handler (NetworkDiscoveryHandler)`: The STUN handler.
    - `ui (NetworkDiscoveryUI)`: The UI object.

    #### Methods:
    - `closeEvent(a0: QCloseEvent | None) -> None`: Overrides the `closeEvent` method in `QWidget`.
    """

    def __init__(self, controller: NetworkController, start: bool = True):
        """
        Initialize the NetworkDiscovery widget.

        Args:
            controller (NetworkController): The network controller.
            start (bool): Whether to start the STUN worker immediately. Default is True.
        """
        super().__init__()
        self.controller = controller

        self.stun_handler = NetworkDiscoveryHandler(self)
        self.ui = NetworkDiscoveryUI(self)

        if start:
            self.stun_handler.start_stun_worker()

    def closeEvent(self, a0: QCloseEvent | None) -> None:
        """
        Overrides the `closeEvent` method in `QWidget`.

        Args:
            a0 (QCloseEvent | None): The close event.
        """
        self.controller.terminate_stun_worker()
        super().closeEvent(a0)
