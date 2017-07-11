# RTOS::Semaphore class

assert('Semaphore class') do
  assert_kind_of(Class, RTOS::Semaphore.class)
  assert_equal(Object, RTOS::Semaphore.superclass)
end

assert('Semaphore.new') do
  assert_nothing_raised {
    s1 = RTOS::Semaphore.new
    s2 = RTOS::Semaphore.new(1)
    s3 = RTOS::Semaphore.new(2, 2)
    s4 = RTOS::Semaphore.new(3, 3, 2)
    s5 = RTOS::Semaphore.new(4, 4, 4, :fifo)
    s6 = RTOS::Semaphore.new(5, 5, 5, :priority)
  }
  assert_raise(ArgumentError) {
    RTOS::Semaphore.new(6, 1, 1, :hoge)
  }
end

assert('Semaphore::ATTRIBUTE') do
  atr = RTOS::Semaphore::ATTRIBUTE
  assert_kind_of(Hash, atr)
  assert_true(atr.include?(:fifo))
  assert_true(atr.include?(:priority))
end

assert('Semaphore#release') do
  assert_nothing_raised {
    RTOS::Semaphore.new.release
  }
end

assert('Semaphore#wait') do
  s = RTOS::Semaphore.new
  assert_nothing_raised {
    s.wait
    s.wait(10)
    s.wait(:forever)
    s.wait(:polling)
  }
  assert_raise(ArgumentError) {
    s.wait(:hoge)
  }
end
