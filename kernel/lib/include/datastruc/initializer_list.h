
namespace std
{
    /// initializer_list
  template<typename E>
    class initializer_list
    {
    public:
      typedef E 		value_type;
      typedef const E& 	reference;
      typedef const E& 	const_reference;
      typedef size_t 		size_type;
      typedef const E* 	iterator;
      typedef const E* 	const_iterator;

    private:
      iterator			m_array;
      size_type			m_len;

      // The compiler can call a private constructor.
      constexpr initializer_list(const_iterator a, size_type l)
      : m_array(a), m_len(l) { }

    public:
      constexpr initializer_list() noexcept
      : m_array(0), m_len(0) { }

      // Number of elements.
      constexpr size_type
      size() const noexcept { return m_len; }

      // First element.
      constexpr const_iterator
      begin() const noexcept { return m_array; }

      // One past the last element.
      constexpr const_iterator
      end() const noexcept { return begin() + size(); }
    };

  /**
   *  @brief  Return an iterator pointing to the first element of
   *          the initializer_list.
   *  @param  ils  Initializer list.
   */
  template<class Tp>
    constexpr const Tp*
    begin(initializer_list<Tp> ils) noexcept
    { return ils.begin(); }

  /**
   *  @brief  Return an iterator pointing to one past the last element
   *          of the initializer_list.
   *  @param  ils  Initializer list.
   */
  template<class Tp>
    constexpr const Tp*
    end(initializer_list<Tp> ils) noexcept
    { return ils.end(); }
}
namespace util
{
    template<typename E>
    using initializer_list = std::initializer_list<E>;
} // namespace util
