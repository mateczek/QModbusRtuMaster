#ifndef REINTERPRETERVARIABLE_H
#define REINTERPRETERVARIABLE_H
//funkcje do zmiany kolejnośći bajtów
#include <QObject>
#include<tuple>

class reinterpreterVariable : public QObject
{
    Q_OBJECT
    uint64_t zmienna;

public:
    explicit reinterpreterVariable(QObject *parent = nullptr);\
    void setVariable(uint64_t zm){
        zmienna = zm;
    }

    std::tuple<int16_t,int16_t,uint16_t,uint16_t> get16bitValue(uint16_t zm);
    std::tuple<int32_t, uint32_t, float, int32_t, uint32_t, float,int32_t, uint32_t,float, int32_t, uint32_t, float> get32bitValue(uint32_t zm);
    std::tuple<int64_t, uint64_t, double, int64_t, uint64_t, double> get64bitValue(uint64_t zm);

signals:
};

#endif // REINTERPRETERVARIABLE_H
