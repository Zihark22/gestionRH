TEMPLATE = subdirs

# Liste des sous-projets
SUBDIRS = \
    app \
    tests

# Définir l'ordre : 'tests' dépend de 'app'
tests.depends = app