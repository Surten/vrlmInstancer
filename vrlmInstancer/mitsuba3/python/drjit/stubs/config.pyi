from typing import Any, Callable, Iterable, Iterator, Tuple, List, TypeVar, Union, overload
import drjit
import drjit as dr

CXX_COMPILER = ...
"""
str(object='') -> str
str(bytes_or_buffer[, encoding[, errors]]) -> str

Create a new string object from the given object. If encoding or
errors is specified, then the object must expose a data buffer
that will be decoded using the given encoding and error handler.
Otherwise, returns the result of object.__str__() (if defined)
or repr(object).
encoding defaults to sys.getdefaultencoding().
errors defaults to 'strict'.
"""
