/* KDevelop QMake Support
 *
 * Copyright 2010 Milian Wolff <mail@milianw.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "test_qmakefile.h"
#include "qmakefile.h"
#include "variablereferenceparser.h"

#include <QTest>

#include <QTextStream>
#include <QProcessEnvironment>
#include <QDebug>
#include <KTemporaryFile>

QTEST_MAIN(TestQMakeFile);

Q_DECLARE_METATYPE(QMakeFile::VariableMap)

namespace QTest {

template<>
char *toString(const QStringList &list)
{
    QByteArray ba;
    if (list.isEmpty()) {
        ba = "()";
    } else {
        ba = "(\"" + list.join("\", \"").toLocal8Bit() + "\")";
    }
    return qstrdup(ba.data());
}

template<>
char *toString(const QMakeFile::VariableMap &variables)
{
    QByteArray ba = "VariableMap(";
    QMap< QString, QStringList >::const_iterator it = variables.constBegin();
    while (it != variables.constEnd()) {
        ba += "[" + it.key() + "] = " + toString(it.value());
        ++it;
        if (it != variables.constEnd()) {
            ba += ", ";
        }
    }
    ba += ")";
    return qstrdup(ba.data());
}

}

void TestQMakeFile::varResolution()
{
    QFETCH(QString, fileContents);
    QFETCH(QMakeFile::VariableMap, variables);

    KTemporaryFile tmpfile;
    tmpfile.open();
    QTextStream stream(&tmpfile);
    stream << fileContents;
    stream << flush;
    tmpfile.close();

    QMakeFile file(tmpfile.fileName());
    QVERIFY(file.read());
    QCOMPARE(file.variableMap(), variables);
}

void TestQMakeFile::varResolution_data()
{
    QTest::addColumn<QString>("fileContents");
    QTest::addColumn< QMakeFile::VariableMap >("variables");

    {
    QMakeFile::VariableMap variables;
    variables["VAR1"] = QStringList() << "1";
    QTest::newRow("simple") << "VAR1 = 1\n"
                            << variables;
    }

    {
    QMakeFile::VariableMap variables;
    variables["VAR1"] = QStringList() << "1";
    variables["VAR2"] = QStringList() << "1";
    QTest::newRow("var-in-var") << "VAR1 = 1\nVAR2 = $$VAR1\n"
                            << variables;
    }
    {
    QMakeFile::VariableMap variables;
    
    variables["VAR1"] = QStringList() << "foo";
    variables["VAR2"] = QStringList() << "foo";
    QTest::newRow("curlyvar") << "VAR1 = foo\nVAR2 = $${VAR1}\n"
                            << variables;
    }
    {
    QMakeFile::VariableMap variables;
    
    variables["VAR1"] = QStringList() << QProcessEnvironment::systemEnvironment().value("USER");
    QTest::newRow("qmakeshell") << "VAR1 = $$(USER)\n"
                            << variables;
    }
    {
    QMakeFile::VariableMap variables;
    
    variables["VAR1"] = QStringList() << "foo";
    variables["VAR2"] = QStringList() << "foo/bar";
    QTest::newRow("path") << "VAR1 = foo\nVAR2 = $$VAR1/bar\n"
                            << variables;
    }
    {
    QMakeFile::VariableMap variables;
    
    variables["VAR_1"] = QStringList() << "foo";
    variables["VAR_2"] = QStringList() << "foo/bar";
    QTest::newRow("var-underscore") << "VAR_1 = foo\nVAR_2 = $$VAR_1/bar"
                            << variables;
    }
}

void TestQMakeFile::referenceParser()
{
    QFETCH(QString, var);

    VariableReferenceParser parser;
    parser.setContent(var);
    QVERIFY(parser.parse());
}


void TestQMakeFile::referenceParser_data()
{
    QTest::addColumn<QString>("var");

    QTest::newRow("dot") << ".";
    QTest::newRow("dotdot") << "..";
}

#include "test_qmakefile.moc"
