repost_horoscope_bot
====================

c code for the repost_horoscope bot

todo:
  1. regex in c
    1a.) learn posix regex
    1b.) parse /r/pics for imgur urls
    1c.) parse karmadecay for oldest original post with 100% match
    1d.) parse the oldest reddit result for its post date in unix time
    1e.) parse a horoscope website (tba)
  2. find out how much of %error from karmadecay is useful in finding oldest post
      this might require grading age vs %match, with something much older being
      allowed to be much more different than the 100% matches, definately need a
      floor for %match though, as some stuff is wildly different.