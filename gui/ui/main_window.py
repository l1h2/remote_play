from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import QMainWindow, QVBoxLayout, QWidget

from controllers import NetworkController
from utils import Defaults

from .widgets import NetworkDiscovery, PeerConnection


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.local_port = Defaults.LOCAL_PORT
        self._init_controllers()
        self._init_ui()

    def closeEvent(self, a0: QCloseEvent | None) -> None:
        self.net_discovery.close()
        self.peer_connection.close()
        super().closeEvent(a0)

    def _init_controllers(self) -> None:
        self.network_controller = NetworkController(self.local_port)

    def _init_ui(self) -> None:
        self._set_window()
        self._set_main_widget()
        self._add_widgets()
        self._set_layout()

    def _set_window(self) -> None:
        self.setWindowTitle("Remote Play App")
        self.resize(600, 300)

    def _set_main_widget(self) -> None:
        self.main_widget = QWidget()
        self.setCentralWidget(self.main_widget)

    def _add_widgets(self) -> None:
        self.net_discovery = NetworkDiscovery(self.network_controller, True)
        self.peer_connection = PeerConnection(self.network_controller)

    def _set_layout(self) -> None:
        self.main_layout = QVBoxLayout(self.main_widget)
        self.main_layout.addWidget(self.net_discovery)
        self.main_layout.addWidget(self.peer_connection)
