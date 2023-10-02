#include "registration.h"
#include "analyse_viewmodel.h"
#include "search_listmodel.h"
#include "search_viewmodel.h"
#include "waffle_listmodel.h"
#include <QQmlEngine>
#include <QMetaType>


namespace capstone::registration {
    static constexpr int kVersionMajor = 1;
    static constexpr int kVersionMinor = 0;
}

void capstone::registration::RegisterTypes() {
    qmlRegisterType<SearchListModel>("CppTypes", kVersionMajor, kVersionMinor, "SearchListModel");
    qmlRegisterType<WaffleListModel>("CppTypes", kVersionMajor, kVersionMinor, "WaffleListModel");

    qmlRegisterSingletonType<SearchViewmodel>(
        "CppTypes", 
        kVersionMajor, 
        kVersionMinor, 
        "SearchViewModel", 
        [](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject* {
            Q_UNUSED(engine);
            Q_UNUSED(scriptEngine);

            return SearchViewmodel::GetInstance();
        }
    );

    qmlRegisterSingletonType<AnalyseViewmodel>(
        "CppTypes", 
        kVersionMajor, 
        kVersionMinor, 
        "AnalyseViewModel", 
        [](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject* {
            Q_UNUSED(engine);
            Q_UNUSED(scriptEngine);

            return AnalyseViewmodel::GetInstance();
        }
    );


}

