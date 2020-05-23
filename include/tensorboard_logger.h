#ifndef TENSORBOARD_LOGGER_H
#define TENSORBOARD_LOGGER_H

#include <exception>
#include <fstream>
#include <string>
#include <vector>

#include "crc.h"
#include "event.pb.h"

using tensorflow::Event;
using tensorflow::Summary;

class TensorBoardLogger {
   public:
    explicit TensorBoardLogger(const char *log_file) {
        bucket_limits_ = nullptr;
        ofs_ = new std::ofstream(
            log_file, std::ios::out | std::ios::trunc | std::ios::binary);
        if (!ofs_->is_open())
            throw std::runtime_error("failed to open log_file " +
                                     std::string(log_file));
    }
    ~TensorBoardLogger() {
        ofs_->close();
        if (bucket_limits_ != nullptr) {
            delete bucket_limits_;
            bucket_limits_ = nullptr;
        }
    }
    int add_scalar(const std::string &tag, int step, float value);
    int add_histogram(const std::string &tag, int step, const float *value,
                      size_t num);
    int add_histogram(const std::string &tag, int step,
                      std::vector<float> &value);
    int add_image(const std::string &tag, int step,
                  const std::string &encoded_image, int height, int width,
                  int channel, const std::string &display_name = "",
                  const std::string &description = "");
    int add_images(const std::string &tag, int step,
                   const std::vector<std::string> &encoded_images,
                   const std::vector<int> &heights,
                   const std::vector<int> &widths,
                   const std::vector<int> &channels,
                   const std::vector<std::string> &display_names,
                   const std::vector<std::string> &descriptions);
    int add_audio(const std::string &tag, int step,
                  const std::string &encoded_audio, float sample_rate,
                  int num_channels, int length_frame,
                  const std::string &content_type,
                  const std::string &display_name = "",
                  const std::string &description = "");
    int add_text(const std::string &tag, int step, const char *text);

   private:
    int generate_default_buckets();
    int add_event(int64_t step, Summary *summary);
    int write(Event &event);
    int create_image_summary(const std::string &tag,
                             const std::string &encoded_image, int height,
                             int width, int channel,
                             const std::string &display_name,
                             const std::string &description, Summary::Value *v);

    std::ofstream *ofs_;
    std::vector<double> *bucket_limits_;
};      // class TensorBoardLogger
#endif  // TENSORBOARD_LOGGER_H
