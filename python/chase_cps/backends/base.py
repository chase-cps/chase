"""Common interface of the back-end adapters."""

from __future__ import annotations


class Adapter:
    """A back end answering some query types.

    ``emit(query)`` returns the input file of the tool, ``run(query)`` calls
    the tool and parses its output into a ``Result``.
    """

    name = "adapter"
    answers: tuple = ()

    def available(self) -> bool:
        raise NotImplementedError

    def emit(self, query) -> str:
        raise NotImplementedError

    def run(self, query):
        raise NotImplementedError

    def __repr__(self) -> str:
        kinds = ", ".join(q.__name__ for q in self.answers)
        return f"<{type(self).__name__} {self.name}: {kinds}>"
