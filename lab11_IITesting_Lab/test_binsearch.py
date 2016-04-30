
from pytest import raises
import numpy as np
from binsearch import binary_search

def test_nan_array():
    assert binary_search([np.nan, 1, 2, 3], np.nan) == 0
    
def test_non_numetric():
    assert binary_search(['a', 'b', 'c', 'd'], 'd') == 3
    
def zero_element():
    assert binary_search([], 3) == -1
    
def one_element():
    assert binary_search([3], 3) == 0
    
def two_element1():
    assert binary_search([1,2], 1) == 0
    
def two_element2():
    assert binary_search([1,2], 2) == 1
    
def needle_out_of_range():
    assert binary_search([11,21,31], 5) == -1
    
def needle_out_of_range2():
    assert binary_search([11,21,31], 55) == -1
    
def needle_inbetween():
    assert binary_search([11,21,31], 12.1) == -1
    
def extreme_needle():
    assert binary_search([11,21,31], np.inf) == -1