#include <QtTest>
#include "include/employee.hpp"

class TestEmployee : public QObject {
    Q_OBJECT


private slots:
    // Exécuté avant/après chaque test (fixtures)
    // void init() {}
    // void cleanup() {}

    // Test classique d'une fonction membre
    void testSetters() {
        Employee e;
        e.setId(5);
        QCOMPARE(e.id(), 5);
        QVERIFY(e.id() == 5);
    }

    // // Test d'un Signal Qt avec QSignalSpy
    // void testSignalDivision() {
    //     Calculatrice calc;

    //     // Espion pour surveiller l'émission du signal
    //     QSignalSpy spyErreur(&calc, &Calculatrice::erreurDivisionParZero);
    //     QSignalSpy spySucces(&calc, &Calculatrice::resultatPret);

    //     // Cas 1 : Division par zéro
    //     calc.diviser(10.0, 0.0);
    //     QCOMPARE(spyErreur.count(), 1); // Doit avoir émis 1 fois
    //     QCOMPARE(spySucces.count(), 0);

    //     // Cas 2 : Division valide
    //     calc.diviser(10.0, 2.0);
    //     QCOMPARE(spySucces.count(), 1);

    //     // Récupérer la valeur transmise dans le signal
    //     QList<QVariant> arguments = spySucces.takeFirst();
    //     QCOMPARE(arguments.at(0).toDouble(), 5.0);
    // }

    // // Data-driven testing (Table de données de test)
    // void testDataDriven_data() {
    //     QTest::addColumn<int>("a");
    //     QTest::addColumn<int>("b");
    //     QTest::addColumn<int>("resultat");

    //     QTest::newRow("positifs") << 2 << 2 << 4;
    //     QTest::newRow("negatifs") << -2 << -3 << -5;
    //     QTest::newRow("zero")     << 0 << 0 << 0;
    // }

    // void testDataDriven() {
    //     QFETCH(int, a);
    //     QFETCH(int, b);
    //     QFETCH(int, resultat);

    //     Calculatrice calc;
    //     QCOMPARE(calc.additionner(a, b), resultat);
    // }

};



// 1. Génère le main()
QTEST_MAIN(TestEmployee)

// 2. INDISPENSABLE : Force Qt à compiler le code MOC pour ce fichier .cpp
#include "TestEmployee.moc"



