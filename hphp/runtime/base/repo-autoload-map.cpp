/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-present Facebook, Inc. (http://www.facebook.com)  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/repo-autoload-map.h"

#include "hphp/runtime/base/builtin-functions.h"
#include "hphp/runtime/vm/repo-file.h"
#include "hphp/util/assertions.h"

TRACE_SET_MOD(repo_autoload);

namespace fs = std::filesystem;

namespace HPHP {

//////////////////////////////////////////////////////////////////////

using UnitToPathMap = tbb::concurrent_hash_map<int64_t, const StringData*>;

namespace {
  // Holds interned absolute paths for symbols we have already looked up.
  UnitToPathMap unitToPathMap;
}

RepoAutoloadMap::RepoAutoloadMap(
      CaseInsensitiveMap types,
      CaseInsensitiveMap functions,
      CaseSensitiveMap constants,
      CaseInsensitiveMap typeAliases,
      CaseSensitiveMap modules)
  : m_types{std::move(types)},
    m_functions{std::move(functions)},
    m_constants{std::move(constants)},
    m_typeAliases{std::move(typeAliases)},
    m_modules{std::move(modules)} {
  if (Trace::moduleEnabled(Trace::repo_autoload, 2)) {
    FTRACE(2, "Type:\n");
    for (auto const DEBUG_ONLY& s : types) {
      FTRACE(2, "{} => {}\n", s.first->data(), s.second);
    }
    FTRACE(2, "Functions:\n");
    for (auto const DEBUG_ONLY& s : functions) {
      FTRACE(2, "{} => {}\n", s.first->data(), s.second);
    }
    FTRACE(2, "Constants:\n");
    for (auto const DEBUG_ONLY& s : constants) {
      FTRACE(2, "{} => {}\n", s.first->data(), s.second);
    }
    FTRACE(2, "TypeAliases:\n");
    for (auto const DEBUG_ONLY& s : typeAliases) {
      FTRACE(2, "{} => {}\n", s.first->data(), s.second);
    }
    FTRACE(2, "Modules:\n");
    for (auto const DEBUG_ONLY& s : modules) {
      FTRACE(2, "{} => {}\n", s.first->data(), s.second);
    }
  }
}

template <typename Compare>
static Optional<String> getPathFromSymbol(
    const RepoAutoloadMap::Map<Compare>& map,
    const String& name) {
  auto search = map.find(name.get());
  if (search == map.end()) {
    FTRACE(1, "Fail autoload {}\n", name.data());
    return {};
  }
  auto unitSn = search->second;

  UnitToPathMap::const_accessor acc;
  if (unitToPathMap.find(acc, unitSn)) {
    FTRACE(1, "Success autoload (cache) {} {}\n", name.data(), acc->second->data());
    return {StrNR(acc->second).asString()};
  }

  auto const path = [&] () -> const StringData* {
    auto const relative = RepoFile::findUnitPath(unitSn);
    always_assert(relative);
    assertx(relative->isStatic());
    if (RO::SourceRoot.empty() || relative->data()[0] == '/') return relative;
    return makeStaticString(RO::SourceRoot + relative->data());
  }();
  unitToPathMap.insert(std::make_pair(unitSn, path));

  FTRACE(1, "Success autoload {} {}\n", name.data(), path->data());
  return {StrNR(path).asString()};
}

template <typename Compare>
static Optional<fs::path> getPathFromSymbol(
    const RepoAutoloadMap::Map<Compare>& map,
    std::string_view name) {
  auto file = getPathFromSymbol(map, StrNR(makeStaticString(name)));
  if (!file) return {};
  return file->toCppString();
}

template <typename Compare>
Array getSymbolFromPath(
    const RepoAutoloadMap::Map<Compare>& map,
    const String& path) {

  auto const unitSn = [&] {
    auto const pathData = path.c_str();
    if (pathData[0] == '/' && !RO::SourceRoot.empty() &&
        !strncmp(RO::SourceRoot.c_str(), pathData, RO::SourceRoot.size())) {
      auto const strippedPath =
        makeStaticString(pathData + RO::SourceRoot.size());
      auto const sn = RepoFile::findUnitSN(strippedPath);
      if (sn >= 0) return sn;
    }
    return RepoFile::findUnitSN(makeStaticString(path));
  }();

  auto ret = Array::CreateVec();
  if (unitSn >= 0) {
    for (auto it = map.begin(); it != map.end(); ++it) {
      if (it->second == unitSn) {
        ret.append(StrNR(it->first).asString());
      }
    }
  }
  return ret;
}

Optional<String>
RepoAutoloadMap::getTypeOrTypeAliasFile(const String& typeName) {
  auto typeFile = getTypeFile(typeName);
  if (typeFile) {
    return typeFile;
  }
  return getTypeAliasFile(typeName);
}

Optional<String>
RepoAutoloadMap::getTypeFile(const String& typeName) {
  return getPathFromSymbol(m_types, typeName);
}

Optional<String>
RepoAutoloadMap::getFunctionFile(const String& funcName) {
  return getPathFromSymbol(m_functions, funcName);
}

Optional<String>
RepoAutoloadMap::getConstantFile(const String& constName) {
  return getPathFromSymbol(m_constants, constName);
}

Optional<String>
RepoAutoloadMap::getTypeAliasFile(const String& typeAliasName) {
  return getPathFromSymbol(m_typeAliases, typeAliasName);
}

Optional<fs::path>
RepoAutoloadMap::getTypeOrTypeAliasFile(std::string_view typeName) {
  auto typeFile = getTypeFile(typeName);
  if (typeFile) {
    return typeFile;
  }
  return getTypeAliasFile(typeName);
}

Optional<String>
RepoAutoloadMap::getModuleFile(const String& moduleName) {
  return getPathFromSymbol(m_modules, moduleName);
}

Optional<fs::path>
RepoAutoloadMap::getTypeFile(std::string_view typeName) {
  return getPathFromSymbol(m_types, typeName);
}

Optional<fs::path>
RepoAutoloadMap::getFunctionFile(std::string_view funcName) {
  return getPathFromSymbol(m_functions, funcName);
}

Optional<fs::path>
RepoAutoloadMap::getConstantFile(std::string_view constName) {
  return getPathFromSymbol(m_constants, constName);
}

Optional<fs::path>
RepoAutoloadMap::getTypeAliasFile(std::string_view typeAliasName) {
  return getPathFromSymbol(m_typeAliases, typeAliasName);
}

Optional<fs::path>
RepoAutoloadMap::getModuleFile(std::string_view moduleName) {
  return getPathFromSymbol(m_modules, moduleName);
}

Array RepoAutoloadMap::getFileTypes(const String& path) {
  return getSymbolFromPath(m_types, path);
}

Array RepoAutoloadMap::getFileFunctions(const String& path) {
  return getSymbolFromPath(m_functions, path);
}

Array RepoAutoloadMap::getFileConstants(const String& path) {
  return getSymbolFromPath(m_constants, path);
}

Array RepoAutoloadMap::getFileTypeAliases(const String& path) {
  return getSymbolFromPath(m_typeAliases, path);
}

Array RepoAutoloadMap::getFileModules(const String& path) {
  return getSymbolFromPath(m_modules, path);
}

AutoloadMap::Result RepoAutoloadMap::handleFailure(
  KindOf kind, const String& className, const Variant& err) const {
  return AutoloadMap::Result::Failure;
}

Array RepoAutoloadMap::getAllFiles() const {
  SystemLib::throwInvalidOperationExceptionObject(
    "Repo Autoload Map does not support getAllFiles"
  );
}

//////////////////////////////////////////////////////////////////////

} // HPHP
