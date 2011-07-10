#ifndef LOSSY_COUNTING
#define LOSSY_COUNTING

#include <map>

template <typename T> 
std::map<T, int> LossyCounting(T *p, int k) {
    int n = 0;
    int delta = 0;
    std::map<T, int> m_count;

    for (off_t len = 0; len < sizeof(p); ++len) {
        n++;
        if (m_count.find(p[len]) == m_count.end()) {
            ++m_count[ p[len] ];
        } else {
            m_count[ p[len] ] = 1 + delta;
        }

        if (n/k != delta) {
            delta = n/k;

            typename std::map<T, int>::iterator it;
            for ( it = m_count.begin() ; it != m_count.end(); ++it ) {
                if ((*it).second < delta) {
                    m_count.erase(it);
                }
            }
        }
    }

    return m_count;	
}

#endif
