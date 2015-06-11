/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef LEDAPROJECTCONSTANTS_H
#define LEDAPROJECTCONSTANTS_H

#include <QtGlobal>

namespace LedaProjectManager {
namespace Constants {

const char PROJECTCONTEXT[]     = "LedaProject.ProjectContext";
const char LEDAMIMETYPE[]    = "text/x-leda-project"; // ### FIXME

// Contexts
const char FILES_EDITOR_ID[]    = "QT4.FilesEditor";

const char FILES_MIMETYPE[]     = "application/vnd.qtcreator.leda.files";
const char INCLUDES_MIMETYPE[]  = "application/vnd.qtcreator.leda.includes";
const char CONFIG_MIMETYPE[]    = "application/vnd.qtcreator.leda.config";

// Project wizard categories
//const char LEDA_PROJECT_WIZARD_CATEGORY[] = "A.LedaProject";
//const char LEDA_PROJECT_WIZARD_CATEGORY_DISPLAY[] = QT_TRANSLATE_NOOP("ProjectExplorer", "LibreEDA Project");

// Project
const char LEDAPROJECT_ID[]  = "LedaProjectManager.LedaProject";

const char HIDE_FILE_FILTER_SETTING[] = "LedaProject/FileFilter";
const char HIDE_FILE_FILTER_DEFAULT[] = "Makefile*; *.o; *.lo; *.la; *.obj; *~; *.files;"
    " *.config; *.creator; *.user; *.includes; *.autosave";

const char SHOW_FILE_FILTER_SETTING[] = "LedaProject/ShowFileFilter";
const char SHOW_FILE_FILTER_DEFAULT[] = "*.c; *.cc; *.cpp; *.cp; *.cxx; *.c++; *.h; *.hh; *.hpp; *.hxx;";

} // namespace Constants
} // namespace LedaProjectManager

#endif // LEDAPROJECTCONSTANTS_H
