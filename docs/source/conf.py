# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
#import os
#import sys
#sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'SLN-TLHMI-IOT'
copyright = '2022, NXP Semiconductors'
author = 'NXP Semiconductors'

# The full version, including alpha/beta/rc tags
release = 'SLN-TLHMI-IOT'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ['myst_parser', 'sphinx_design']
numpydoc_show_class_members = False
source_suffix = ['.rst', '.md']
suppress_warnings = ['myst.reference']
source_parsers = {
   '.md': 'recommonmark.parser.CommonMarkParser',
}
myst_enable_extensions = [
    "amsmath",
    "colon_fence",
    "deflist",
    "dollarmath",
    "fieldlist",
    "html_admonition",
    "html_image",
    "linkify",
    "replacements",
    "smartquotes",
    "substitution",
    "tasklist",
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
#exclude_patterns = [
#        'bootloader/_security/**',
#        'bootloader/boot_modes/_over_the_air_updates.md',
#        'bootloader/boot_modes/_over_the_wire_updates.md']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'collapse_navigation': False
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = []

xtensions = ['rst2pdf.pdfbuilder']


master_doc = "index"

latex_elements = {'figure_align': 'H',}

# add documents needed for latexpdf generation
#latex_documents: This value determines how to group the document tree into LaTeX source files. It must be a list of tuples (s#tartdocname, targetname, title, author, theme, toctree_only), where the items are:
latex_documents = [('index', "SLN-TLHMI-IOT SW Developer Guide.tex", "SLN-TLHMI-IOT SW Developer Guide", "NXP Semiconductors", "manual")]

