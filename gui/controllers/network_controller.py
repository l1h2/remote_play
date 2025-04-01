from collections.abc import Callable
from typing import cast

from utils import Network
from workers import StunQueryWorker, UdpClientWorker, UdpPeerWorker, UdpServerWorker

from .worker_controller import WorkerController


class NetworkController:
    """
    ### A class to manage network workers.

    #### Attributes:
    - `local_port (int)`: The local port to be used by the network workers.

    #### Properties:
    - `stun_worker`: The STUN worker instance.
    - `udp_peer_worker`: The UDP peer worker instance.
    - `udp_server_worker`: The UDP server worker instance.
    - `udp_client_worker`: The UDP client worker instance.

    #### Methods:
    - `start_stun_worker(callback: Callable[[str], None])`: Start the STUN worker subprocess.
    - `stop_stun_worker()`: Stop the STUN worker subprocess.
    - `terminate_stun_worker()`: Terminate the STUN worker thread.
    - `start_udp_peer_worker(network: Network, callback: Callable[[str], None])`: Start the UDP peer worker subprocess.
    - `stop_udp_peer_worker()`: Stop the UDP peer worker subprocess.
    - `terminate_udp_peer_worker()`: Terminate the UDP peer worker thread.
    - `start_udp_server_worker(network: Network, callback: Callable[[str], None])`: Start the UDP server worker subprocess.
    - `stop_udp_server_worker()`: Stop the UDP server worker subprocess.
    - `terminate_udp_server_worker()`: Terminate the UDP server worker thread.
    - `start_udp_client_worker(network: Network, callback: Callable[[str], None])`: Start the UDP client worker subprocess.
    - `stop_udp_client_worker()`: Stop the UDP client worker subprocess.
    - `terminate_udp_client_worker()`: Terminate the UDP client worker thread.
    - `stop_workers()`: Stop all worker subprocesses.
    - `terminate_workers()`: Terminate all worker threads.
    """

    def __init__(self, local_port: int) -> None:
        """
        Initialize the NetworkController with a local port.

        Args:
            local_port (int): The local port to be used by the network workers.
        """
        self.local_port = local_port
        self._stun_worker = WorkerController(StunQueryWorker)
        self._udp_peer_worker = WorkerController(UdpPeerWorker)
        self._udp_server_worker = WorkerController(UdpServerWorker)
        self._udp_client_worker = WorkerController(UdpClientWorker)

    @property
    def stun_worker(self) -> StunQueryWorker | None:
        """
        Get the STUN worker instance.

        Returns:
            StunQueryWorker | None: The STUN worker instance or None if not initialized.
        """
        return cast(StunQueryWorker | None, self._stun_worker.worker)

    @property
    def udp_peer_worker(self) -> UdpPeerWorker | None:
        """
        Get the UDP peer worker instance.

        Returns:
            UdpPeerWorker | None: The UDP peer worker instance or None if not initialized.
        """
        return cast(UdpPeerWorker | None, self._udp_peer_worker.worker)

    @property
    def udp_server_worker(self) -> UdpServerWorker | None:
        """
        Get the UDP server worker instance.

        Returns:
            UdpServerWorker | None: The UDP server worker instance or None if not initialized.
        """
        return cast(UdpServerWorker | None, self._udp_server_worker.worker)

    @property
    def udp_client_worker(self) -> UdpClientWorker | None:
        """
        Get the UDP client worker instance.

        Returns:
            UdpClientWorker | None: The UDP client worker instance or None if not initialized.
        """
        return cast(UdpClientWorker | None, self._udp_client_worker.worker)

    def start_stun_worker(self, callback: Callable[[str], None]) -> None:
        """
        Start the STUN worker subprocess if it is not already running.\
        
        Args:
            callback (Callable[[str], None]): Callback function to handle the output from the STUN worker.
        """
        self._stun_worker.start(Network(self.local_port), callback)

    def stop_stun_worker(self) -> None:
        """
        Stop the STUN worker subprocess if it is running.
        """
        self._stun_worker.stop()

    def terminate_stun_worker(self) -> None:
        """
        Terminate the STUN worker thread and clean up resources.
        """
        self._stun_worker.terminate()

    def start_udp_peer_worker(
        self, network: Network, callback: Callable[[str], None]
    ) -> None:
        """
        Start the UDP peer worker subprocess if it is not already running.

        Args:
            network (Network): The network instance to be used by the UDP peer worker.
            callback (Callable[[str], None]): Callback function to handle the output from the UDP peer worker.
        """
        self._udp_peer_worker.start(network, callback)

    def stop_udp_peer_worker(self) -> None:
        """
        Stop the UDP peer worker subprocess if it is running.
        """
        self._udp_peer_worker.stop()

    def terminate_udp_peer_worker(self) -> None:
        """
        Terminate the UDP peer worker thread and clean up resources.
        """
        self._udp_peer_worker.terminate()

    def start_udp_server_worker(
        self, network: Network, callback: Callable[[str], None]
    ) -> None:
        """
        Start the UDP server worker subprocess if it is not already running.

        Args:
            network (Network): The network instance to be used by the UDP server worker.
            callback (Callable[[str], None]): Callback function to handle the output from the UDP server worker.
        """
        self._udp_server_worker.start(network, callback)

    def stop_udp_server_worker(self) -> None:
        """
        Stop the UDP server worker subprocess if it is running.
        """
        self._udp_server_worker.stop()

    def terminate_udp_server_worker(self) -> None:
        """
        Terminate the UDP server worker thread and clean up resources.
        """
        self._udp_server_worker.terminate()

    def start_udp_client_worker(
        self, network: Network, callback: Callable[[str], None]
    ) -> None:
        """
        Start the UDP client worker subprocess if it is not already running.

        Args:
            network (Network): The network instance to be used by the UDP client worker.
            callback (Callable[[str], None]): Callback function to handle the output from the UDP client worker.
        """
        self._udp_client_worker.start(network, callback)

    def stop_udp_client_worker(self) -> None:
        """
        Stop the UDP client worker subprocess if it is running.
        """
        self._udp_client_worker.stop()

    def terminate_udp_client_worker(self) -> None:
        """
        Terminate the UDP client worker thread and clean up resources.
        """
        self._udp_client_worker.terminate()

    def stop_workers(self) -> None:
        """
        Stop all worker subprocesses if they are running.
        """
        self.stop_stun_worker()
        self.stop_udp_peer_worker()
        self.stop_udp_server_worker()
        self.stop_udp_client_worker()

    def terminate_workers(self) -> None:
        """
        Terminate all worker threads and clean up resources.
        """
        self.terminate_stun_worker()
        self.terminate_udp_peer_worker()
        self.terminate_udp_server_worker()
        self.terminate_udp_client_worker()
