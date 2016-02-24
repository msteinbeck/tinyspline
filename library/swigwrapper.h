#pragma once

#include <vector>
#include <string>
#include <stdexcept>

/* NOTE:
 *
 * All method names start with 'ts_' to prevent clashes with method/function
 * names of target languages.
 *
 * Due to the fact that some of the target languages do not support size_t
 * we need to use int instead.
 *
 */

namespace ts {

class FloatList {
public:
    FloatList() {}
    FloatList(const std::vector<float>& vec) : vec(vec) {}

    float ts_get(const int index) const {
        if (index < 0) {
            std::string text = "Negative index: " + std::to_string(index);
            throw std::out_of_range(text);
        }
        return vec.at(index);
    }

    float ts_set(const int index, const float value) {
        if (index < 0) {
            std::string text = "Negative index: " + std::to_string(index);
            throw std::out_of_range(text);
        }
        return vec.at(index) = value;
    }

    void ts_insert(const int index, const float value) {
        if (index < 0) {
            std::string text = "Negative index: " + std::to_string(index);
            throw std::out_of_range(text);
        }
        vec.insert(vec.begin() + index, value);
    }

    float ts_remove(const int index) {
        if (index < 0) {
            std::string text = "Negative index: " + std::to_string(index);
            throw std::out_of_range(text);
        }
        const float val = vec.at(index);
        vec.erase(vec.begin() + index);
        return val;
    }

    bool ts_remove(const float value) {
        const int idx = ts_indexOf(value);
        if (idx >= 0)
            ts_remove(idx);
        return idx >= 0;
    }

    void ts_clear() {
        vec.clear();
    }

    int ts_indexOf(const float value) const {
        const int size = ts_size();
        for (int i = 0; i < size; i++) {
            if (ts_fequals(vec.at(i), value))
                return i;
        }
        return -1;
    }

    bool ts_contains(const float value) const {
        return ts_indexOf(value) >= 0;
    }

    bool ts_isEmpty() const {
        return vec.empty();
    }

    int ts_size() const {
        const int size = static_cast<int>(vec.size());
        if (size < 0) {
            std::string text = "The size of this container exceeds the"
                " maximum value of an int.";
            throw std::runtime_error(text);
        }
        return size;
    }

    std::string ts_toString() const {
        std::string s = "[";
        const int size = ts_size();
        if (size > 0) {
            const size_t lst = size - 1; /* must be >= 0 */
            for (size_t i = 0; i < lst; i++)
                s += std::to_string(vec.at(i)) + ", ";
            s += std::to_string(vec.at(lst));
        }
        s += "]";
        return s;
    }

private:
    std::vector<float> vec;
};

}
