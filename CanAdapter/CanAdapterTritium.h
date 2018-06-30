#ifndef CANADAPTERTRITIUM_H
#define CANADAPTERTRITIUM_H

#include "CanAdapter.h"

class CanHub;
class CanHandle;

class CanAdapterTritium : public CanAdapter
{
    Q_OBJECT

public:
    CanAdapterTritium(CanHub &canHub);
    ~CanAdapterTritium() override;

    bool open() override;
    void close() override;

    bool isOpen() override;

    QWidget * getControlWidget(QWidget *parent = 0) override;

private slots:
    void openClicked();
    void closeClicked();

    void transmit(can_message_t cmsg);

private:
    CanHandle * m_canHandle;
};

#endif // CANADAPTERTRITIUM_H
