from collections.abc import Callable
from enum import Enum

from utils import Network
from workers import (
    StunQueryWorker,
    SubprocessNetWorker,
    UdpClientWorker,
    UdpPeerWorker,
    UdpServerWorker,
)


class Workers(Enum):
    stun_worker = "stun_worker"
    udp_peer_worker = "udp_peer_worker"
    udp_server_worker = "udp_server_worker"
    udp_client_worker = "udp_client_worker"


class NetworkController:
    def __init__(self, local_port: int) -> None:
        self.local_port = local_port
        self.stun_worker: StunQueryWorker | None = None
        self.udp_peer_worker: UdpPeerWorker | None = None
        self.udp_server_worker: UdpServerWorker | None = None
        self.udp_client_worker: UdpClientWorker | None = None

    def start_stun_worker(self, callback: Callable[[str], None]) -> None:
        self._start_worker(
            Workers.stun_worker,
            StunQueryWorker,
            Network(self.local_port),
            callback,
        )

    def stop_stun_worker(self) -> None:
        self._stop_worker(Workers.stun_worker)

    def start_udp_peer_worker(
        self, network: Network, callback: Callable[[str], None]
    ) -> None:
        self._start_worker(
            Workers.udp_peer_worker,
            UdpPeerWorker,
            network,
            callback,
        )

    def stop_udp_peer_worker(self) -> None:
        self._stop_worker(Workers.udp_peer_worker)

    def start_udp_server_worker(
        self, network: Network, callback: Callable[[str], None]
    ) -> None:
        self._start_worker(
            Workers.udp_server_worker,
            UdpServerWorker,
            network,
            callback,
        )

    def stop_udp_server_worker(self) -> None:
        self._stop_worker(Workers.udp_server_worker)

    def start_udp_client_worker(
        self, network: Network, callback: Callable[[str], None]
    ) -> None:
        self._start_worker(
            Workers.udp_client_worker,
            UdpClientWorker,
            network,
            callback,
        )

    def stop_udp_client_worker(self) -> None:
        self._stop_worker(Workers.udp_client_worker)

    def stop_workers(self) -> None:
        self.stop_stun_worker()
        self.stop_udp_peer_worker()
        self.stop_udp_server_worker()
        self.stop_udp_client_worker()

    def _start_worker(
        self,
        worker_attr: Workers,
        worker_class: type,
        network: Network,
        callback: Callable[[str], None],
    ) -> None:
        worker: SubprocessNetWorker | None = getattr(self, worker_attr.value)
        if worker and worker.listening:
            return

        new_worker: SubprocessNetWorker = worker_class(network)
        new_worker.output.connect(callback)
        new_worker.start()
        setattr(self, worker_attr.value, new_worker)

    def _stop_worker(self, worker_attr: Workers) -> None:
        worker: SubprocessNetWorker | None = getattr(self, worker_attr.value)
        if not worker or not worker.listening:
            return

        worker.terminate_process()
        worker.wait()
