#pragma once

namespace swarcs::accretion::graphics {

    /**
     * @brief RAII wrapper for an OpenGL shader program object.
     *
     * Automatically handles shader program creation and ensures safe deletion via glDeleteProgram
     * upon destruction if ownership is not released.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    class ProgramHandle {
    private:
        unsigned int id{0}; ///< Raw OpenGL program identifier.

    public:
        /**
         * @brief Constructs and creates an OpenGL shader program object.
         * @throws std::runtime_error if program creation fails.
         */
        ProgramHandle();

        /**
         * @brief Destructor releasing the OpenGL program resource if still owned.
         */
        ~ProgramHandle();

        // Delete copy operations
        ProgramHandle(const ProgramHandle&) = delete;
        ProgramHandle& operator=(const ProgramHandle&) = delete;

        // Enable move semantics
        ProgramHandle(ProgramHandle&& other) noexcept;
        ProgramHandle& operator=(ProgramHandle&& other) noexcept;

        /**
         * @brief Retrieves the raw OpenGL program identifier.
         * @return unsigned int The program ID.
         */
        [[nodiscard]] unsigned int get() const;

        /**
         * @brief Releases ownership of the program ID without deleting it.
         * @return unsigned int The released program ID.
         */
        unsigned int release();
    };

} // namespace swarcs::accretion::graphics