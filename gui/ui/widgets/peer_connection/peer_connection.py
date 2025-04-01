from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import QWidget

from controllers import NetworkController
from utils import Network

from .handlers import PeerConnectionHandler, StreamHandler
from .ui import PeerConnectionUI


class PeerConnection(QWidget):
    """
    ### Widget for establishing a connection to a peer.

    #### Attributes:
    - `controller (NetworkController)`: The network controller.
    - `network (Network)`: The network object.
    - `connection_handler (PeerConnectionHandler)`: The connection handler.
    - `stream_handler (StreamHandler)`: The stream handler.
    - `ui (PeerConnectionUI)`: The UI object.

    #### Methods:
    - `closeEvent(a0: QCloseEvent | None) -> None`: Overrides the `closeEvent` method in `QWidget`.
    """

    def __init__(self, controller: NetworkController):
        super().__init__()
        self.controller = controller
        self.network = Network(controller.local_port)

        self.connection_handler = PeerConnectionHandler(self)
        self.stream_handler = StreamHandler(self)
        self.ui = PeerConnectionUI(self)

    def closeEvent(self, a0: QCloseEvent | None) -> None:
        """
        Overrides the `closeEvent` method in `QWidget`.

        Args:
            a0 (QCloseEvent | None): The close event.
        """
        self.controller.terminate_workers()
        return super().closeEvent(a0)
