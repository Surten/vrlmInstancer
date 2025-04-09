from typing import Any, Callable, Iterable, Iterator, Tuple, List, TypeVar, Union, overload
import drjit
import drjit as dr

def _loop_process_state(value: type, in_state: list, out_state: list, write: bool, in_struct: bool = False):
    """
    
    This helper function is used by ``drjit.*.Loop`` to collect the set of loop
    state variables and ensure that their types stay consistent over time. It
    traverses a python object tree in ``value`` and writes state variable
    indices to ``out_state``. If provided, it performs a consistency check
    against the output of a prior call provided via ``in_state``. If ``write``
    is set to ``True``, it mutates the input value based on the information in
    ``in_state``.
    
    """
    ...

def _wraps(wrapped, assigned=('__module__', '__name__', '__qualname__', '__doc__', '__annotations__'), updated=('__dict__',)):
    """
    Decorator factory to apply update_wrapper() to a wrapper function
    
    Returns a decorator that invokes update_wrapper() with the decorated
    function as the wrapper argument and the arguments to wraps() as the
    remaining arguments. Default arguments are as for update_wrapper().
    This is a convenience function to simplify applying partial() to
    update_wrapper().
    
    """
    ...

def apply_cpp(arg, func, diff=True):
    """
    
    Apply a C++ function to data structures containing JIT variables. The C++
    lambda must take as input the index of the JIT variable. It can optionally
    return a new index in which case a new data structure will be constructed
    carrying the new JIT variables.
    
    """
    ...

def array_configure(cls, shape, type_, value):
    """
    Populates an Dr.Jit array class with extra type trait fields
    """
    ...

def array_from_dlpack(t, capsule): ...
def array_init(self, args):
    """
    
    This generic initialization routine initializes an arbitrary Dr.Jit array
    from a variable-length argument list (which could be a scalar broadcast, a
    component list, or a NumPy/PyTorch/Tensorflow array..)
    
    """
    ...

def array_name(prefix, vt, shape, scalar):
    """
    
    Determines the name of an array (e.g. Float32, ArrayXf32, etc.). This
    function is used when arrays are created during initialization of the Dr.Jit
    extension module, and during implicit type promotion where array types are
    determined dynamically.
    
    Parameter ``prefix`` (``str``):
    Array flavor prefix (Array/Matrix/Complex/Quaternion)
    
    Parameter ``vt`` (``drjit.VarType``):
    Underlying scalar type (e.g. ``VarType.Int32``) of the desired array
    
    Parameter ``shape`` (``Tuple[int]``):
    Size per dimension
    
    Parameter ``scalar`` (``bool``):
    Arrays in the ``drjit.scalar.*`` module use a different depth
    convention, which is indicated via this parameter.
    
    """
    ...

def diff_vars(arg, indices, check_grad_enabled=True):
    """
    
    Extract indices of differentiable variables.
    
    """
    ...

@overload
def eval(arg0: int) -> bool: ...
@overload
def eval() -> None: ...
def fmadd_scalar(arg0: float, arg1: float, arg2: float) -> float: ...
def get_args_values(f, *args, **kwargs):
    """
    
    Given a function, a tuple of positional arguments and a dict of keyword
    arguments, return the full tuple of positional arguments, including default
    values and keyword arguments at the right position.
    
    Here is a simple example:
    
    def f(a, b, c=1, d=2):
    pass
    get_args_values(f, 6, 5, d=4) # returns (4, 5, 1, 4)
    
    """
    ...

def idiv(arg0: drjit.VarType, arg1: object) -> object: ...
def loop_process_state(loop, funcs, state, write): ...
class reinterpret_flag:
    def __init__(self: drjit.detail.reinterpret_flag) -> None: ...
    ...

def reinterpret_scalar(arg0: object, arg1: drjit.VarType, arg2: drjit.VarType) -> object: ...
def schedule(arg0: int) -> bool: ...
def slice_tensor(shape, indices, uint32):
    """
    
    This function takes an array shape (integer tuple) and a tuple containing
    slice indices. It returns the resulting array shape and a flattened 32-bit
    unsigned integer array containing element indices.
    
    """
    ...

def sub_len(o): ...
def tensor_getitem(tensor, slice_arg): ...
def tensor_init(tensor_type, obj): ...
def tensor_setitem(tensor, slice_arg, value): ...
def traverse(traverse_static_array=True, traverse_tensor=True):
    """
    
    Function decorator that traverses nested datastructures (e.g. dicts, lists,
    Dr.Jit struct, ...) and applies the decorated function to all JIT arrays
    it encounters.
    
    """
    ...

