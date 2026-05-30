#ifndef RESULT_H
#define RESULT_H

struct Result {
    bool success = false;
    QVariant data;
    QString error;
};

#endif // RESULT_H
