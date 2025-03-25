from collections.abc import Callable

from utils import Network, Subprocess
from workers import StunQueryWorker, SubprocessNetWorker, UdpPeerWorker


class NetworkController:
    def __init__(self, local_port: int) -> None:
        self.local_port = local_port
        self.stun_worker = None
        self.udp_worker = None

    def start_stun_worker(self, callback: Callable[[str], None]) -> None:
        if self.stun_worker and self.stun_worker.listening:
            return

        self.stun_worker = StunQueryWorker(Subprocess.STUN_CLIENT, self.local_port)
        self.stun_worker.output.connect(callback)
        self.stun_worker.start()

    def stop_stun_worker(self) -> None:
        if not self.stun_worker:
            return

        self._stop_worker(self.stun_worker)

    def start_udp_worker(
        self, network: Network, callback: Callable[[str], None]
    ) -> None:
        if self.udp_worker and self.udp_worker.listening:
            return

        self.udp_worker = UdpPeerWorker(Subprocess.UDP_CONNECTION, network)
        self.udp_worker.output.connect(callback)
        self.udp_worker.start()

    def stop_udp_worker(self) -> None:
        if not self.udp_worker:
            return

        self._stop_worker(self.udp_worker)

    def stop_workers(self) -> None:
        self.stop_stun_worker()
        self.stop_udp_worker()

    def _stop_worker(self, worker: SubprocessNetWorker) -> None:
        if not worker.listening:
            return

        worker.terminate_process()
        worker.wait()
