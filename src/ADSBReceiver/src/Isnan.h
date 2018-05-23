#ifndef ISNAN
#define ISNAN

namespace std {
    template< class T>
    bool isnan(const T& numb) {
        return numb!=numb;
    }
}
#endif // ISNAN

