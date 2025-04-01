from typing import TYPE_CHECKING

from PyQt6.QtWidgets import QApplication

if TYPE_CHECKING:
    from .network_discovery import NetworkDiscovery


class NetworkDiscoveryHandler:
    """
    ### Handler class for the NetworkDiscovery widget.

    #### Methods:
    - `start_stun_worker`: Start the STUN worker.
    - `copy_to_clipboard`: Copy label text to clipboard.
    """

    def __init__(self, widget: "NetworkDiscovery"):
        """
        Initialize the handler class.

        Args:
            widget (NetworkDiscovery): The NetworkDiscovery widget.
        """
        self._widget = widget

    def start_stun_worker(self) -> None:
        """
        Start the STUN worker.
        """
        self._widget.controller.start_stun_worker(self._update_network)

    def copy_to_clipboard(self) -> None:
        """
        Copy label text to clipboard.
        """
        clipboard = QApplication.clipboard()
        if clipboard:
            clipboard.setText(self._widget.ui.label.text())

    def _update_network(self, output: str) -> None:
        """
        Update the network object with socket string.

        Args:
            output (str): The socket string to update the network object with.
        """
        self._widget.ui.label.setText(output)
